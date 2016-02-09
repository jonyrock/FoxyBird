//
//  JNIResults.cpp
//  RescueGiant
//
//  Created by AppleProgrammer on 03.06.15.
//
//

#include "JNIResults.h"

using namespace SonarCocosHelper;

JNIEXPORT void JNICALL Java_sonar_systems_frameworks_Chartboost_ChartBoostAds_rewardVideowasViewedChartboost(JNIEnv* env, jobject thiz, jboolean result)
{
    //CPP code here
}

JNIEXPORT void JNICALL Java_sonar_systems_frameworks_Chartboost_ChartBoostAds_FullscreenAdPreloaded(JNIEnv* env, jobject thiz, jboolean result)
{
    //CPP code here
	Chartboost::showFullscreenAd();
}
