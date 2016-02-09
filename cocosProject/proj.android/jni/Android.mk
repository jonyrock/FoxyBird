LOCAL_PATH := $(call my-dir)
COCOS_HELPER_ROOT := $(COCOS_ROOT)/../Cocos-Helper

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared
LOCAL_MODULE_FILENAME := libgame_cpp

rwildcard=$(foreach d, $(wildcard $1*), $(call rwildcard, $d/, $2) $(filter $(subst *, %, $2), $d))

LOCAL_SRC_FILES := $(patsubst jni/%, %, $(call rwildcard, jni/, *.cpp))
LOCAL_SRC_FILES += $(patsubst %, ../%, $(call rwildcard, ../Game/, *.cpp))
LOCAL_SRC_FILES += ../../SonarHelper/SonarFrameworks.cpp
LOCAL_SRC_FILES += ../../SonarHelper/JNIHelpers.cpp
LOCAL_SRC_FILES += ../../SonarHelper/JNIResults.cpp
LOCAL_SRC_FILES += ../../CCLocalNotification/LocalNotification_Android.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Game
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../external
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../SonarHelper/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../CCLocalNotification/

LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_STATIC_LIBRARIES += cocos_ui_static
LOCAL_STATIC_LIBRARIES += cocos_network_static
LOCAL_STATIC_LIBRARIES += cocos2dx_store_static
LOCAL_STATIC_LIBRARIES += leveldb_static
LOCAL_STATIC_LIBRARIES += cocos_helper_static

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(COCOS_ROOT))
$(call import-add-path, $(COCOS_ROOT)/external)
$(call import-add-path, $(COCOS_ROOT)/cocos)
$(call import-add-path, $(LOCAL_PATH)/../../external)
$(call import-add-path, $(LOCAL_PATH)/../../)
$(call import-add-path, $(COCOS_HELPER_ROOT)/../)

$(call import-module, .)
$(call import-module, audio/android)
$(call import-module, ui)
$(call import-module, network)
$(call import-module, extensions)
$(call import-module, extensions/cocos2dx-store)
$(call import-module, Cocos-Helper/android)
$(call import-module, leveldb/prebuild/android)





