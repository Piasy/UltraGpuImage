package com.piasy.ugi.filters;

import com.piasy.ugi.UgiFilter;

/**
 * Created by Piasy{github.com/Piasy} on 2018/12/22.
 */
public class UgiFilterGroup extends UgiFilter {
    public UgiFilterGroup() {
        super(FILTER_GROUP);
    }

    private static native void nativeAddFilter(long handle, long filterHandle);

    public void addFilter(UgiFilter filter) {
        if (mNativeHandle != 0 && filter.getNativeHandle() != 0) {
            nativeAddFilter(mNativeHandle, filter.getNativeHandle());
        }
    }
}
