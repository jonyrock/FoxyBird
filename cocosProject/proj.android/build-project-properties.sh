#!/bin/bash

TARGET_FILE="project.properties"
echo "target=android-19" > ${TARGET_FILE}
cocosPath=$(./relative-path.sh ${COCOS_ROOT})
cocosPath=${cocosPath}/cocos/platform/android/java/
echo "android.library.reference.1=${cocosPath}" >> ${TARGET_FILE}
googlePlayPath=$(./relative-path.sh ${ANDROID_SDK_ROOT})
googlePlayPath=${googlePlayPath}/extras/google/google_play_services/libproject/google-play-services_lib
echo "android.library.reference.2=${googlePlayPath}" >> ${TARGET_FILE}