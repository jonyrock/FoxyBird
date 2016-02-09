//
//  JNIResults.h
//  RescueGiant
//
//  Created by AppleProgrammer on 03.06.15.
//
//

#ifndef __JNIResults__
#define __JNIResults__

#include <stdio.h>

#include "cocos2d.h"
#include "SonarFrameworks.h"

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>


extern "C" {

    JNIEXPORT void JNICALL Java_sonar_systems_frameworks_Chartboost_ChartBoostAds_rewardVideowasViewedChartboost(JNIEnv* env, jobject thiz, jboolean result);

    JNIEXPORT void JNICALL Java_sonar_systems_frameworks_Chartboost_ChartBoostAds_FullscreenAdPreloaded(JNIEnv* env, jobject thiz, jboolean result);
};

#endif /* defined(__JniHelpers_H__) */

