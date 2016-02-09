LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := snappy_static
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libsnappy.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := leveldb_static
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libleveldb.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_STATIC_LIBRARIES := snappy_static
include $(PREBUILT_STATIC_LIBRARY)