LOCAL_PATH:= $(call my-dir)/../

#================================================================
#	K	G	E
#================================================================

include $(CLEAR_VARS)

  LOCAL_MODULE := kge
   LOCAL_SRC_FILES := ../../KGEAndroid/libs/armeabi/libkge.so
   include $(PREBUILT_SHARED_LIBRARY)

#================================================================
#	T	E	S	T	S
#================================================================
include $(CLEAR_VARS)
LOCAL_MODULE := memorytest
LOCAL_SRC_FILES := 	main.cpp
LOCAL_SHARED_LIBRARIES := kge
include $(BUILD_SHARED_LIBRARY)
