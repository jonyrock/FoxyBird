
#include "Utils.h"

#include <platform/android/jni/JniHelper.h>
#include <jni.h>
#include <cocos/platform/android/jni/JniHelper.h>

using namespace cocos2d;

using namespace Utils;

void openUrlTry(const char* url, const char* url2) {
    JniMethodInfo minfo;
    if(JniHelper::getStaticMethodInfo(
	minfo, 
	"org/cocos2dx/lib/Cocos2dxActivity", 
	"openWebURLTried", 
	"(Ljava/lang/String;Ljava/lang/String;)V")) {
	
	jstring StringArg1 = minfo.env->NewStringUTF(url);
	jstring StringArg2 = minfo.env->NewStringUTF(url2);
	minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, StringArg1, StringArg2);
	minfo.env->DeleteLocalRef(StringArg1);
	minfo.env->DeleteLocalRef(StringArg2);
	minfo.env->DeleteLocalRef(minfo.classID);
	
    } else {
	CCLOG("Can't get jni method to open url!");
    }
   
}

void Utils::openMarket() {
   
    // TODO: special call to open market
    // as here http://stackoverflow.com/questions/11753000/how-to-open-the-google-play-store-directly-from-my-android-application
    
    openUrlTry(
	"market://details?id=com.jonyrock.Catalina",
	"https://play.google.com/store/apps/details?id=com.jonyrock.Catalina"
    );

}

void Utils::openUrl(const char* url) {
    
    JniMethodInfo minfo;
    if(JniHelper::getStaticMethodInfo(
	minfo, 
	"org/cocos2dx/lib/Cocos2dxActivity", 
	"openWebURL", 
	"(Ljava/lang/String;)V")) {
	
	jstring StringArg1 = minfo.env->NewStringUTF(url);
	minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, StringArg1);
	minfo.env->DeleteLocalRef(StringArg1);
	minfo.env->DeleteLocalRef(minfo.classID);
	
    } else {
	CCLOG("Can't get jni method to open url!");
    }

}
