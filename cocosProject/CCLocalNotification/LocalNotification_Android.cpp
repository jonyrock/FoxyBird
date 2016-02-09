#include "LocalNotification.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <cocos2d.h>

using namespace std;

// Java class
#define CLASS_NAME "org/cocos2dx/cpp/AppActivity"

static bool getJNIStaticMethodInfo(cocos2d::JniMethodInfo &methodinfo,
                                        const char *methodName,
                                        const char *paramCode) {
  return cocos2d::JniHelper::getStaticMethodInfo(methodinfo,
                                                 CLASS_NAME,
                                                 methodName,
                                                 paramCode);
}

void LocalNotification::show(std::string message, int interval, int tag)
{
    CCLOG("INFO: show message");
    cocos2d::JniMethodInfo methodInfo;
    
    if (!getJNIStaticMethodInfo(methodInfo, "showLocalNotification", "(Ljava/lang/String;II)V")) {
        CCLOG("WARN: can'f find show method!");
        return;
    }

    jstring stringArg = methodInfo.env->NewStringUTF(message.c_str());
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg, interval, tag);
    methodInfo.env->DeleteLocalRef(stringArg);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void LocalNotification::cancel(int tag)
{
  cocos2d::JniMethodInfo methodInfo;
  
  if (! getJNIStaticMethodInfo(methodInfo, "cancelLocalNotification", "(I)V")) {
    return;
  }

  methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, tag);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

bool LocalNotification::askAboutNotification() {
  return true;
}
