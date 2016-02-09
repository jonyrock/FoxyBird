LOCAL_PATH := $(call my-dir)

# cocos2dx-soomla-common
include $(CLEAR_VARS)

COCOS2D_JAVASCRIPT = $(filter %-DCOCOS2D_JAVASCRIPT=1,$(APP_CPPFLAGS))

LOCAL_MODULE := cocos2dx_soomla_common_static
LOCAL_MODULE_FILENAME := libcocos2dxsoomlacommon

CORE_SRC_LIST := $(wildcard $(LOCAL_PATH)/Soomla/*.cpp)
CORE_SRC_LIST += $(wildcard $(LOCAL_PATH)/Soomla/domain/*.cpp)
CORE_SRC_LIST += $(wildcard $(LOCAL_PATH)/Soomla/data/*.cpp)
CORE_SRC_LIST += $(wildcard $(LOCAL_PATH)/Soomla/rewards/*.cpp)
CORE_SRC_LIST += $(wildcard $(LOCAL_PATH)/Soomla/NativeImpl/*.cpp)
ifneq '$(COCOS2D_JAVASCRIPT)' ''
CORE_SRC_LIST += $(wildcard $(LOCAL_PATH)/Soomla/jsb/*.cpp)
endif

LOCAL_SRC_FILES := $(CORE_SRC_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/Soomla
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/data
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/domain
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/rewards
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/NativeImpl
ifneq '$(COCOS2D_JAVASCRIPT)' ''
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/jsb
endif

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2d_js_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos2d_simulator_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += jansson_static


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla/data
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla/domain
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla/rewards
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla/NativeImpl
ifneq '$(COCOS2D_JAVASCRIPT)' ''
LOCAL_EXPORT_C_INCLUDES += $$(LOCAL_PATH)/Soomla/jsb
endif

include $(BUILD_STATIC_LIBRARY)

ifneq '$(COCOS2D_JAVASCRIPT)' ''
$(call import-module,jansson)
$(call import-module,scripting/js-bindings/proj.android/prebuilt-mk)
$(call import-module,tools/simulator/libsimulator/proj.android/prebuilt-mk)
else
$(call import-module,external/jansson)
endif
