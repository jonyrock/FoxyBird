LOCAL_PATH := $(call my-dir)

# cocos2dx-store
include $(CLEAR_VARS)

COCOS2D_JAVASCRIPT = $(filter %-DCOCOS2D_JAVASCRIPT=1,$(APP_CPPFLAGS))

LOCAL_MODULE := cocos2dx_store_static
LOCAL_MODULE_FILENAME := libcocos2dxstore

STORE_SRC_LIST := $(wildcard $(LOCAL_PATH)/Soomla/*.cpp)
STORE_SRC_LIST += $(wildcard $(LOCAL_PATH)/Soomla/NativeImpl/*.cpp)
STORE_SRC_LIST += $(wildcard $(LOCAL_PATH)/Soomla/PurchaseTypes/*.cpp)
STORE_SRC_LIST += $(wildcard $(LOCAL_PATH)/Soomla/data/*.cpp)
STORE_SRC_LIST += $(wildcard $(LOCAL_PATH)/Soomla/domain/*.cpp)
STORE_SRC_LIST += $(wildcard $(LOCAL_PATH)/Soomla/domain/virtualCurrencies/*.cpp)
STORE_SRC_LIST += $(wildcard $(LOCAL_PATH)/Soomla/domain/virtualGoods/*.cpp)
STORE_SRC_LIST += $(wildcard $(LOCAL_PATH)/Soomla/rewards/*.cpp)

LOCAL_SRC_FILES := $(STORE_SRC_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/Soomla
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/data
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/domain
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/domain/virtualCurrencies
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/domain/virtualGoods
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/jsb
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/PurchaseTypes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/NativeImpl
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/rewards
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../lib \
        $(LOCAL_PATH)/../../../bindings/manual

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_soomla_common_static

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla/data
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla/domain
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla/domain/virtualCurrencies
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla/domain/virtualGoods
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla/jsb
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla/PurchaseTypes
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla/rewards

include $(BUILD_STATIC_LIBRARY)

ifneq '$(COCOS2D_JAVASCRIPT)' ''
$(call import-module,soomla-cocos2dx-core)
else
$(call import-module,extensions/soomla-cocos2dx-core)
endif

