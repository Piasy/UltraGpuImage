package com.piasy.ugi.filters;

import com.piasy.ugi.UgiFilter;

/**
 * Created by Piasy{github.com/Piasy} on 2018/12/22.
 */
public class UgiBrightnessFilter extends UgiFilter {
    public UgiBrightnessFilter() {
        super(FILTER_BRIGHTNESS);
    }

    private static native void nativeSetBrightness(long handle, float brightness);

    public void setBrightness(float brightness) {
        if (mNativeHandle != 0) {
            nativeSetBrightness(mNativeHandle, brightness);
        }
    }
}
