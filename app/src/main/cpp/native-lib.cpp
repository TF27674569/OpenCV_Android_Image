#include <jni.h>
#include <string>

#include <android/bitmap.h>
#include <opencv2/opencv.hpp>
#include "cv_helper.h"

#define VERTICAL 0
#define HORIZONTAL 1


using namespace cv;

/**
 * 竖向的逆世界效果
 * 图像竖向分为4等份，中间一部分取出（1/2大小）放原图下面，上面1/2以镜像效果显示
 */
extern "C"
JNIEXPORT jobject JNICALL
Java_com_ndk_sample_NDKBitmapUtils_againstWorld(JNIEnv *env, jclass type, jobject bitmap,
                                                jint orientation) {
    // 基于OpenCV
    // 1.bitmap转Mat
    Mat src;
    int code = cv_helper::bitmap2Mat(env, bitmap, src);
    CV_Assert(code >= 0);
    // 结果图像
    Mat des(src.size(), src.type());

    int h = src.rows;
    int w = src.cols;

    if (orientation == HORIZONTAL) {
        int against_piece = w >> 2;//竖向分为4小块

        for (int i = 0; i < h; ++i) {//行
            for (int j = 0; j < w; ++j) {
                if (j < (w >> 1)) {
                    // 这里默认做ARGB8888处理
                    des.at<Vec4b>(i, j) = src.at<Vec4b>(i, w - against_piece - j);
                } else {
                    des.at<Vec4b>(i, j) = src.at<Vec4b>(i, j - against_piece);
                }
            }
        }
    } else {
        int against_piece = h >> 2;//竖向分为4小块

        for (int i = 0; i < h; ++i) {//行
            for (int j = 0; j < w; ++j) {
                if (i < (h >> 1)) {
                    // 这里默认做ARGB8888处理
                    des.at<Vec4b>(i, j) = src.at<Vec4b>(h - i - against_piece, j);
                } else {
                    des.at<Vec4b>(i, j) = src.at<Vec4b>(i - against_piece, j);
                }
            }
        }
    }

    cv_helper::mat2Bitmap(env, des, bitmap);
    return bitmap;
}

/**
 * 浮雕效果
 */
extern "C"
JNIEXPORT jobject JNICALL
Java_com_ndk_sample_NDKBitmapUtils_anaglyph(JNIEnv *env, jclass type, jobject bitmap) {
    Mat src;
    cv_helper::bitmap2Mat(env, bitmap, src);

    /**
     * [1,0]
     * [0,-1]
     * 使用此卷积核进行卷积，并将BGR的值加上128
     */
    Mat des(src.size(), src.type());

    // 从第2行第2个开始进行操作，防止 index益处
    for (int i = 1; i < src.rows; ++i) {
        for (int j = 1; j < src.cols; ++j) {
            // 读RBGA
            Vec4b pix_p = src.at<Vec4b>(i - 1, j - 1);
            Vec4b pix_n = src.at<Vec4b>(i, j);

            //b g r
            des.at<Vec4b>(i, j)[0] = static_cast<uchar>(pix_p[0] - pix_n[0] + 128);
            des.at<Vec4b>(i, j)[1] = static_cast<uchar>(pix_p[1] - pix_n[1] + 128);
            des.at<Vec4b>(i, j)[2] = static_cast<uchar>(pix_p[2] - pix_n[2] + 128);
        }
    }

    cv_helper::mat2Bitmap(env, des, bitmap);

    return bitmap;
}


/**
 * 马赛克
 * 将某个区域的图片用改图片的第一个点的像素值代替
 * 这里n*n的像素块，每个n*n的快用第一个像素的像素值代替
 */
extern "C"
JNIEXPORT jobject JNICALL
Java_com_ndk_sample_NDKBitmapUtils_mosaic(JNIEnv *env, jclass type, jobject bitmap) {
    Mat src;
    cv_helper::bitmap2Mat(env, bitmap, src);

    Mat des(src.size(), src.type());
    int size = 16;// size*size的块进行马赛克

    int h = src.rows;
    int w = src.cols;

    for (int i = 0; i < h - size; i += size) {
        for (int j = 0; j < w - size; j += size) {

            // 8*8的小块
            for (int k = 0; k < size; ++k) {
                for (int l = 0; l < size; ++l) {
                    des.at<Vec4b>(i + k, j + l) = src.at<Vec4b>(i, j);
                }
            }

        }
    }

    cv_helper::mat2Bitmap(env, des, bitmap);
    return bitmap;
}


/**
 * 毛玻璃效果
 * 与马赛克效果相似，将图片分成n*n的小块，然后随机取小块中的一个值填充这个小块
 */
extern "C"
JNIEXPORT jobject JNICALL
Java_com_ndk_sample_NDKBitmapUtils_groundGlass(JNIEnv *env, jclass type, jobject bitmap) {
    Mat src;
    cv_helper::bitmap2Mat(env, bitmap, src);

    Mat des(src.size(), src.type());
    int size = 6;// size*size的块进行毛玻璃

    int h = src.rows;
    int w = src.cols;

    RNG rng(time(NULL));

    for (int i = 0; i < h - size; i += size) {
        for (int j = 0; j < w - size; j += size) {

            // 小块
            for (int k = 0; k < size; ++k) {
                for (int l = 0; l < size; ++l) {
                    int round = rng.uniform(0, size);
                    des.at<Vec4b>(i + k, j + l) = src.at<Vec4b>(i + round, j + round);
                }
            }

        }
    }
    cv_helper::mat2Bitmap(env, des, bitmap);
    return bitmap;
}

/**
 * 油画效果
 * 是通过像素权重实现图像的像素模糊从而达到近似油画效果模糊，（直方统计）
 * 具体的
 *  1. 每个点需要分成 n*n 小块
 *  2. 统计灰度等级
 *  3. 选择灰度等级中最多的值
 *  4. 找到所有的像素取平均值 用颜色值除以灰度统计最大的等级出现的次数
 */
extern "C"
JNIEXPORT jobject JNICALL
Java_com_ndk_sample_NDKBitmapUtils_oilPainting(JNIEnv *env, jclass type, jobject bitmap) {

    Mat src;
    cv_helper::bitmap2Mat(env, bitmap, src);

    // 灰度值均分成 n个区间
    Mat gray;
    cvtColor(src, gray, COLOR_BGRA2GRAY);

    Mat des = src.clone();

    // 将像素点分成8个等级
    int rang = 8;
    int size = 12;

    int h = src.rows;
    int w = src.cols;

    // 剪掉rang
    for (int i = 0; i < h - size; ++i) {
        for (int j = 0; j < w - size; ++j) {
            int g[8] = {0};// 灰度等级

            int b_sum[8] = {0};// b颜色分量的和
            int g_sum[8] = {0};// g颜色分量的和
            int r_sum[8] = {0};// r颜色分量的和

            // 统计size区域内的灰度分布，并求出各个颜色通道的分量和
            for (int k = 0; k < size; ++k) {
                for (int l = 0; l < size; ++l) {
                    // 获取灰度值
                    uchar gery = gray.at<uchar>(i + k, j + l);
                    // 分成8个等级但是index只能是[0-1] 所以需要-1
                    int index = gery * (rang - 1) / 255;
                    g[index] += 1;

                    // 统计各个通道颜色分量的和
                    Vec4b color = src.at<Vec4b>(i + k, j + l);
                    b_sum[index] += color[0];
                    g_sum[index] += color[1];
                    r_sum[index] += color[2];
                }
            }


            // 找出灰度等级最大的
            int max = g[0];// 哪个等级
            int max_index = 0;
            for (int q = 1; q < rang; ++q) {
                if (max < g[q]) {
                    max = g[q];
                    max_index = q;
                }
            }

            // 取平均值赋值给各个颜色通道
            des.at<Vec4b>(i, j)[0] = static_cast<uchar>(b_sum[max_index] / max);
            des.at<Vec4b>(i, j)[1] = static_cast<uchar>(g_sum[max_index] / max);
            des.at<Vec4b>(i, j)[2] = static_cast<uchar>(r_sum[max_index] / max);

        }
    }

    cv_helper::mat2Bitmap(env, des, bitmap);
    return bitmap;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_ndk_sample_NDKBitmapUtils_garyOptimize(JNIEnv *env, jclass type, jobject bitmap) {

    Mat src;
    cv_helper::bitmap2Mat(env, bitmap, src);
    Mat dst(src.size(), CV_8UC1);
    cvtColor(src, dst, cv::COLOR_BGRA2GRAY);
    cvtColor(dst, src, cv::COLOR_GRAY2BGRA);
    return bitmap;
}