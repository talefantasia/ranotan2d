LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ranotan2d_shared

LOCAL_MODULE_FILENAME := libranotan2d

CPP_FILES := $(shell find $(LOCAL_PATH)/../../Classes -name *.cpp)
LOCAL_SRC_FILES := hellocpp/main.cpp
LOCAL_SRC_FILES += $(CPP_FILES:$(LOCAL_PATH)/%=%)
LOCAL_C_INCLUDES := $(shell find $(LOCAL_PATH)/../../Classes -type d)

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
#LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += stdlib_static

LOCAL_CPPFLAGS += -fexceptions

LOCAL_CFLAGS := -std=c++11 -DDEBUG -DCOCOS2D_DEBUG=1 -DSERVICE_ALPHA

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
#$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)

#$(call import-module,CocosDenshion/android) \
#$(call import-module,cocos2dx) \
#$(call import-module,stdlib) \
#$(call import-module,extensions)
