#!/bin/bash

RH_VER=$1

if [ -z $RH_VER ]; then
	echo "No version specified. Please visit http://www.airspayce.com/mikem/arduino/RadioHead to find current version. "
	exit 1
fi

wget http://www.airspayce.com/mikem/arduino/RadioHead/RadioHead-${RH_VER}.zip
unzip -o RadioHead-${RH_VER}.zip

rm RadioHead-${RH_VER}.zip

