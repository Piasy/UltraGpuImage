package com.piasy.ugi;

import androidx.annotation.IntDef;
import com.piasy.ugi.utils.Logging;

/**
 * Created by Piasy{github.com/Piasy} on 2018/12/19.
 */
public class UgiTransformation {

    public static final int COORDS_SCALE = 10000;

    public static final int FLIP_NONE = 0;
    public static final int FLIP_HORIZONTAL = 1;
    public static final int FLIP_VERTICAL = 2;
    public static final int FLIP_HORIZONTAL_VERTICAL = 3;

    public static final int ROTATION_0 = 0;
    public static final int ROTATION_90 = 90;
    public static final int ROTATION_180 = 180;
    public static final int ROTATION_270 = 270;

    public static final int SCALE_TYPE_FIT_XY = 0;
    public static final int SCALE_TYPE_CENTER_CROP = 1;
    public static final int SCALE_TYPE_CENTER_INSIDE = 2;

    private static final String TAG = "UgiTransformation";

    long mNativeHandle;

    UgiTransformation(int inputWidth, int inputHeight, int outputWidth, int outputHeight) {
        mNativeHandle = nativeCreate(inputWidth, inputHeight, outputWidth, outputHeight);
    }

    private static native long nativeCreate(int inputWidth, int inputHeight,
            int outputWidth, int outputHeight);

    private static native void nativeUpdateInput(long handle, int width, int height);

    private static native void nativeUpdateOutput(long handle, int width, int height);

    private static native void nativeUpdateCrop(long handle, int x, int y, int width, int height);

    private static native void nativeUpdateFlip(long handle, int flip);

    private static native void nativeUpdateRotation(long handle, int rotation);

    private static native void nativeUpdateScaleType(long handle, int scaleType);

    private static native int nativeGetInputWidth(long handle);

    private static native int nativeGetInputHeight(long handle);

    private static native int nativeGetRotation(long handle);

    private static native int nativeGetFlip(long handle);

    public static native void nativeDestroy(long handle);

    public void updateInput(int width, int height) {
        Logging.info(TAG, "updateInput " + width + "x" + height);
        if (mNativeHandle != 0) {
            nativeUpdateInput(mNativeHandle, width, height);
        }
    }

    public void updateOutput(int width, int height) {
        Logging.info(TAG, "updateOutput " + width + "x" + height);
        if (mNativeHandle != 0) {
            nativeUpdateOutput(mNativeHandle, width, height);
        }
    }

    public void updateCrop(int x, int y, int width, int height) {
        Logging.info(TAG, "updateCrop (" + x + ", " + y + ", " + width + ", " + height + ")");
        if (mNativeHandle != 0
            && 0 <= x && x <= COORDS_SCALE
            && 0 <= y && y <= COORDS_SCALE
            && 0 <= width && width <= COORDS_SCALE
            && 0 <= height && height <= COORDS_SCALE) {
            nativeUpdateCrop(mNativeHandle, x, y, width, height);
        }
    }

    public void updateFlip(@Flip int flip) {
        Logging.info(TAG, "updateFlip " + flip);
        if (mNativeHandle != 0) {
            nativeUpdateFlip(mNativeHandle, flip);
        }
    }

    public void updateRotation(@Rotation int rotation) {
        Logging.info(TAG, "updateRotation " + rotation);
        if (mNativeHandle != 0) {
            nativeUpdateRotation(mNativeHandle, rotation);
        }
    }

    public void updateScaleType(@ScaleType int scaleType) {
        Logging.info(TAG, "updateScaleType " + scaleType);
        if (mNativeHandle != 0) {
            nativeUpdateScaleType(mNativeHandle, scaleType);
        }
    }

    public int getInputWidth() {
        return mNativeHandle == 0 ? 0 : nativeGetInputWidth(mNativeHandle);
    }

    public int getInputHeight() {
        return mNativeHandle == 0 ? 0 : nativeGetInputHeight(mNativeHandle);
    }

    public int getRotation() {
        return mNativeHandle == 0 ? 0 : nativeGetRotation(mNativeHandle);
    }

    public int getFlip() {
        return mNativeHandle == 0 ? 0 : nativeGetFlip(mNativeHandle);
    }

    void destroy() {
        if (mNativeHandle != 0) {
            nativeDestroy(mNativeHandle);
            mNativeHandle = 0;
        }
    }

    @IntDef(value = {
            FLIP_NONE,
            FLIP_HORIZONTAL,
            FLIP_VERTICAL,
            FLIP_HORIZONTAL_VERTICAL,
    })
    @interface Flip {
    }

    @IntDef(value = {
            ROTATION_0,
            ROTATION_90,
            ROTATION_180,
            ROTATION_270,
    })
    @interface Rotation {
    }

    @IntDef(value = {
            SCALE_TYPE_FIT_XY,
            SCALE_TYPE_CENTER_CROP,
            SCALE_TYPE_CENTER_INSIDE,
    })
    @interface ScaleType {
    }
}
