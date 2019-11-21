//
// Created by TIAN FENG on 2019/11/20.
//

#ifndef NDK_SAMPLE_CV_HELPER_H
#define NDK_SAMPLE_CV_HELPER_H

#include <jni.h>
#include "opencv2/opencv.hpp"
#include <android/bitmap.h>

class cv_helper {
public:
    static int bitmap2Mat(JNIEnv *env, jobject &bitmap, cv::Mat &dst);

    static int mat2Bitmap(JNIEnv *env, cv::Mat &src, jobject &bitmap);
};

#endif //NDK_SAMPLE_CV_HELPER_H
