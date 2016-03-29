//
// Created by Bryan Yap on 01/02/2016.
//

#include <string.h>
#include <jni.h>
#include <opencv2/opencv.hpp>
#include <android/log.h>
#include <android/bitmap.h>

#define  LOG_TAG    "playingwithopencv"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

struct Image {
  uint32_t width;
  uint32_t height;
  int cvImageType;
  void* pixels;
};

void applyFilter(Image sourceImage, Image destinationImage, float intensity) {
  cv::Mat source(cv::Size(sourceImage.width, sourceImage.height), sourceImage.cvImageType, sourceImage.pixels, cv::Mat::AUTO_STEP);
  cv::Mat destination(cv::Size(destinationImage.width, destinationImage.height), destinationImage.cvImageType, destinationImage.pixels, cv::Mat::AUTO_STEP);
  cv::Mat temp;

  float cappedIntensity = intensity;
  cappedIntensity = (cappedIntensity > 1.0) ? 1.0 : cappedIntensity;
  cappedIntensity = (cappedIntensity < 0.0) ? 0.0 : cappedIntensity;

  cv::GaussianBlur(source, destination, cv::Size(11, 11), 0, 0);
  int lowerBound = (int)(cappedIntensity * 255.0);

  cv::cvtColor(source, temp, CV_RGBA2GRAY);
  cv::threshold(temp, temp, lowerBound, 255, 0);
  cv::cvtColor(temp, destination, CV_GRAY2RGBA);
}

int androidToOpenCVImageFormat(int32_t format) {
  switch (format) {
    case ANDROID_BITMAP_FORMAT_RGBA_8888:
      LOGI("androidToOpenCVImageFormat() encountered a supported format! ANDROID_BITMAP_FORMAT_RGBA_8888");
      return CV_8UC4;

    case ANDROID_BITMAP_FORMAT_RGBA_4444:
      LOGE("androidToOpenCVImageFormat() encountered an unexpected format! ANDROID_BITMAP_FORMAT_RGBA_4444");
      return 0;

    case ANDROID_BITMAP_FORMAT_RGB_565:
      LOGE("androidToOpenCVImageFormat() encountered an unexpected format! ANDROID_BITMAP_FORMAT_RGB_565");
      return 0;

    case ANDROID_BITMAP_FORMAT_A_8:
      LOGE("androidToOpenCVImageFormat() encountered an unexpected format! ANDROID_BITMAP_FORMAT_A_8");
      return 0;

    case ANDROID_BITMAP_FORMAT_NONE:
      LOGE("androidToOpenCVImageFormat() encountered an unexpected format! ANDROID_BITMAP_FORMAT_NONE");
      return 0;
  }
}

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jstring JNICALL Java_com_example_bryan_playingwithopencv_MainActivity_stringFromJNI(JNIEnv *env,
                                                                            jclass thiz) {
    cv::Mat mat(33, 22, 1);
    return env->NewStringUTF("Hello from JNI! Compiled straight to your app!");
}

JNIEXPORT void JNICALL Java_com_example_bryan_playingwithopencv_MainActivity_applyFilter(JNIEnv *env, jclass self, jobject source, jobject destination, jfloat intensity) {
    int errorCode;

    LOGI("applyFilter() was called");

    AndroidBitmapInfo info;
    if ((errorCode = AndroidBitmap_getInfo(env, source, &info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed for source image! error = %d", errorCode);
        return;
    }

    LOGI("applyFilter() was loaded source image info successfully (%d x %d [%d])", info.width, info.height, info.stride);

    AndroidBitmapInfo destinationInfo;
    if ((errorCode = AndroidBitmap_getInfo(env, source, &destinationInfo)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed for destination image! error = %d", errorCode);
        return;
    }

    LOGI("applyFilter() was loaded destination image info successfully (%d x %d [%d])", destinationInfo.width, destinationInfo.height, destinationInfo.stride);

    void* pixels = 0;
    if ((errorCode = AndroidBitmap_lockPixels(env, source, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed for the source image! error = %d", errorCode);
    } else {
        LOGI("applyFilter() loaded source image pixels successfully");
    }

    void* outputPixels = 0;
    if ((errorCode = AndroidBitmap_lockPixels(env, destination, &outputPixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed for the destination image! error = %d", errorCode);
    } else {
        LOGI("applyFilter() loaded destination image pixels successfully");
    }

    if (pixels != 0 && outputPixels != 0) {
      Image sourceImage;
      sourceImage.width = info.width;
      sourceImage.height = info.height;
      sourceImage.pixels = pixels;
      sourceImage.cvImageType = androidToOpenCVImageFormat(info.format);

      Image destinationImage;
      destinationImage.width = destinationInfo.width;
      destinationImage.height = destinationInfo.height;
      destinationImage.pixels = outputPixels;
      destinationImage.cvImageType = androidToOpenCVImageFormat(destinationInfo.format);

      LOGI("applyFilter() is now attempting to apply the actual filter");
      applyFilter(sourceImage, destinationImage, (float)intensity);
    } else {
        LOGE("applyFilter() could not find either the source or image pixels");
    }

    if ((errorCode = AndroidBitmap_unlockPixels(env, source)) < 0) {
        LOGE("AndroidBitmap_unlockPixels() failed for the source image! error = %d", errorCode);
    } else {
        LOGI("applyFilter() unloaded source image pixels successfully");
    }

    if ((errorCode = AndroidBitmap_unlockPixels(env, destination)) < 0) {
        LOGE("AndroidBitmap_unlockPixels() failed for the destination image! error = %d", errorCode);
    } else {
        LOGI("applyFilter() unloaded destination image pixels successfully");
    }
}

#ifdef __cplusplus
};
#endif
