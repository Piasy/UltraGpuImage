#!/bin/bash

rm -rf UltraGpuImage/UltraGpuImage.xcodeproj \
    UltraGpuImage-iOS/UltraGpuImage-iOS.xcodeproj \
    iOSExample/iOSExample.xcodeproj \
    iOSExample/iOSExample.xcworkspace && \
cd UltraGpuImage && xcodegen && cd .. && \
cd UltraGpuImage-iOS && xcodegen && cd .. && \
cd iOSExample && xcodegen && pod install && cd .. && \
rm -rf iOSExample/iOSExample.xcworkspace
