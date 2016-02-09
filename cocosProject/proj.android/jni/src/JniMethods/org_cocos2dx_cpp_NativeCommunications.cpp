
#include "org_cocos2dx_cpp_NativeCommunications.h"
#include "../Managers/AdvertisingManagerJni.h"
#include <cocos2d.h>

using namespace cocos2d;

extern "C" {
    JNIEXPORT void JNICALL
    Java_org_cocos2dx_cpp_NativeCommunications_setBottomAdHeight
    (JNIEnv *, jclass, jint height) {
	AdvertisingManagerJni::getInstance()->setBottomHeight(height);
    }
}