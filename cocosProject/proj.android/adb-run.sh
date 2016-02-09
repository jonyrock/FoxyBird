#!/bin/bash

if [ "$(adb shell dumpsys power | grep mScreenOn= | grep -oE '(true|false)')" == false ] ; then
    echo "Screen is off. Turning on."
    adb shell input keyevent 26 # wakeup
    #adb shell input touchscreen swipe 930 380 1080 380 # unlock
    #echo "OK, should be on now."
fi

pkg=$(aapt dump badging $1|awk -F" " '/package/ {print $2}'|awk -F"'" '/name=/ {print $2}')
act=$(aapt dump badging $1|awk -F" " '/launchable-activity/ {print $2}'|awk -F"'" '/name=/ {print $2}')
adb shell am start -n ${pkg}/${act}