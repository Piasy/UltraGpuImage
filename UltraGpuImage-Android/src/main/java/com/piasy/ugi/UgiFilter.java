package com.piasy.ugi;

import androidx.annotation.IntDef;

/**
 * Created by Piasy{github.com/Piasy} on 2018/12/22.
 *
 * Warning: instance can't be reused for multiple renderer.
 */
public class UgiFilter {
    public static final int FILTER_GROUP = 0;
    public static final int FILTER_PREPROCESS = 1;
    public static final int FILTER_COLOR_INVERT = 2;
    public static final int FILTER_BRIGHTNESS = 3;

    protected long mNativeHandle;

    public UgiFilter(final @FilterType int filterType) {
        mNativeHandle = nativeCreate(filterType);
    }

    private static native long nativeCreate(int filterType);

    public long getNativeHandle() {
        return mNativeHandle;
    }

    void releaseHandle() {
        mNativeHandle = 0;
    }

    @IntDef(value = {
            FILTER_GROUP,
            FILTER_PREPROCESS,
            FILTER_COLOR_INVERT,
            FILTER_BRIGHTNESS,
    })
    @interface FilterType {
    }
}
