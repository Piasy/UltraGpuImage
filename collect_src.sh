#!/usr/bin/env bash

REPO=/Users/piasy/src/media/webrtc_repo/webrtc_ios/src

cp ${REPO}/sdk/android/src/java/org/webrtc/EglBase10.java \
    ${REPO}/sdk/android/src/java/org/webrtc/EglBase14.java \
    UltraGpuImage-Android/src/main/java/com/piasy/ugi/egl/ && \
cp ${REPO}/sdk/android/api/org/webrtc/EglBase.java \
    UltraGpuImage-Android/src/main/java/com/piasy/ugi/egl/ && \
cp ${REPO}/rtc_base/java/src/org/webrtc/ThreadUtils.java \
    UltraGpuImage-Android/src/main/java/com/piasy/ugi/utils/
