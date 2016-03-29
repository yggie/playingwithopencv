LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

OPENCVROOT := /Users/bryan/Downloads/OpenCV-android-sdk
OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED
include ${OPENCVROOT}/sdk/native/jni/OpenCV.mk

LOCAL_SRC_FILES := hello-jni.cpp
LOCAL_LDLIBS += -llog -landroid -ljnigraphics
LOCAL_MODULE := hello-jni

include $(BUILD_SHARED_LIBRARY)
