//
// Created by TIAN FENG on 2019/11/20.
//

#include "cv_helper.h"


int cv_helper::bitmap2Mat(JNIEnv *env, jobject &bitmap, cv::Mat &dst) {
    AndroidBitmapInfo bitmapInfo;
    // 获取bitmap的信息
    int infoRes = AndroidBitmap_getInfo(env, bitmap, &bitmapInfo);
    if (infoRes < 0) {
        // 获取失败返回失败信息
        return infoRes;
    }

    // 锁住画布
    void *pixels;
    int lockRes = AndroidBitmap_lockPixels(env, bitmap, &pixels);
    if (lockRes < 0) {
        return lockRes;
    }

    // 根据bitmap的格式来创建对应的Mat
    if (bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        dst.create(bitmapInfo.height, bitmapInfo.width, CV_8UC4);
    } else if (bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        dst.create(bitmapInfo.height, bitmapInfo.width, CV_8UC3);
    } else {
        // 不存在的格式 4444 暂时不做处理
        return -1;
    }

    // 拷贝数据到Mat的data
    dst.data = static_cast<uchar *>(pixels);
    // 解锁画布
    return AndroidBitmap_unlockPixels(env, bitmap);
}

int cv_helper::mat2Bitmap(JNIEnv *env, cv::Mat &src, jobject &bitmap) {
    AndroidBitmapInfo bitmapInfo;

    // 获取bitmap的信息
    int infoRes = AndroidBitmap_getInfo(env, bitmap, &bitmapInfo);
    if (infoRes < 0) {
        // 获取失败返回失败信息
        return infoRes;
    }

    void *pixels;
    // 取出bitmap的像素数据到pixels里面
    int lockRes = AndroidBitmap_lockPixels(env, bitmap, &pixels);
    if (lockRes < 0) {
        return lockRes;
    }

    if (bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        cv::Mat temp(bitmapInfo.height, bitmapInfo.width, CV_8UC4, pixels);
        switch (src.type()) {
            case CV_8UC1:// 灰度图
                cvCvtColor(&src, &temp, cv::COLOR_GRAY2RGBA);
                break;
            case CV_8UC3://RGB565
                cvCvtColor(&src, &temp, cv::COLOR_BGR5652RGBA);
                break;
            case CV_8UC4://RGBA
                src.copyTo(temp);
                break;
        }
    } else if (bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        cv::Mat temp(bitmapInfo.height, bitmapInfo.width, CV_8UC3, pixels);
        switch (src.type()) {
            case CV_8UC1:// 灰度图
                cvCvtColor(&src, &temp, cv::COLOR_GRAY2BGR565);
                break;
            case CV_8UC3://RGB565
                cvtColor(src, temp, cv::COLOR_RGB2BGR565);
                break;
            case CV_8UC4://RGBA
                cvCvtColor(&src, &temp, cv::COLOR_BGRA2BGR565);
                break;
        }
    } else {
        // 不存在的格式
        return -1;
    }
    AndroidBitmap_unlockPixels(env, bitmap);
    return 0;
}
