# This project file has been automatically generated.
# Manual edition is not recommended.

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := kge

WINGDB_FOLDER_NAME := "Source Files"

WINGDB_FOLDER_SRC_FILES := \
    ../../KGEmain/Android.cpp \
    ../../KGEmain/KgeMemoryTrack.cpp \
    ../../KGEmain/KgeMutex.cpp \
    ../../KGEmain/KgeNedmalloc.cpp \
    ../../KGEmain/KgeThread.cpp \
    ../../KGEmain/Logger.cpp \


LOCAL_SRC_FILES := \
    ../../KGEmain/KgeMemoryTrack.cpp \
    ../../KGEmain/KgeMutex.cpp \
    ../../KGEmain/KgeNedmalloc.cpp \
    ../../KGEmain/KgeThread.cpp \
    ../../KGEmain/Logger.cpp \
    ../../KGEmain/Android.cpp \


LOCAL_LDLIBS := -llog
LOCAL_EXPORT_LDLIBS := 

include $(BUILD_SHARED_LIBRARY)


