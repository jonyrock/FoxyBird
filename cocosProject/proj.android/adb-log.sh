#!/bin/bash
PACKAGE=$1
APPPID=""
echo "Waiting for start ${PACKAGE} ..."
while [ -z ${APPPID} ]
do
APPPID=`adb -d shell ps | grep "${PACKAGE}" | cut -c10-15`
sleep 0.3
done

APPPID=${APPPID//[[:space:]]/} #remove spaces

echo "${PACKAGE} started (${APPPID})"

adb logcat | grep "${APPPID})"
