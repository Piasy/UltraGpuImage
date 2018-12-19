package com.piasy.ugi;

import android.graphics.SurfaceTexture;
import android.opengl.EGLContext;
import android.opengl.GLES20;
import android.os.Handler;
import android.os.HandlerThread;
import android.view.Surface;
import com.piasy.ugi.egl.EglBase;
import com.piasy.ugi.egl.EglBase14;
import com.piasy.ugi.utils.Logging;
import com.piasy.ugi.utils.ThreadUtils;

/**
 * Created by Piasy{github.com/Piasy} on 2018/12/17.
 */
public class UgiRenderer {
    private static final String TAG = "UgiRenderer";

    static {
        System.loadLibrary("UltraGpuImage");
    }

    private final String mName;
    private final Handler mRenderHandler;
    private final EglSurfaceCreation mEglSurfaceCreationRunnable = new EglSurfaceCreation();

    private EglBase mEglBase;

    private long mNativeHandle;

    public UgiRenderer(EGLContext sharedContext, int[] configAttributes) {
        this("", sharedContext, configAttributes);
    }

    public UgiRenderer(String name, EGLContext sharedContext, int[] configAttributes) {
        mName = name;
        mNativeHandle = nativeCreate();

        logInfo("new UgiRenderer, handle " + mNativeHandle);

        HandlerThread renderThread = new HandlerThread("UgiRenderer-" + name);
        renderThread.start();
        mRenderHandler = new Handler(renderThread.getLooper());

        // Create EGL context on the newly created render thread. It should be possibly to create
        // the context on this thread and make it current on the render thread, but this causes
        // failure on some Marvel based JB devices.
        // https://bugs.chromium.org/p/webrtc/issues/detail?id=6350.
        ThreadUtils.invokeAtFrontUninterruptibly(mRenderHandler,
                () -> mEglBase = EglBase.create(
                        sharedContext == null ? null : new EglBase14.Context(sharedContext),
                        configAttributes));
        logInfo("new UgiRenderer success");
    }

    private static native long nativeCreate();

    private static native void nativeOnSurfaceChanged(long handle, int width, int height);

    private static native void nativeOnSurfaceDestroyed(long handle);

    private static native void nativeRenderFrame(long handle, int textureId, int width, int height,
            long timestamp);

    private static native void nativeDestroy(long handle);

    public void onSurfaceCreated(Surface surface) {
        logInfo("onSurfaceCreated " + surface);
        createEglSurfaceInternal(surface);
    }

    public void onSurfaceCreated(SurfaceTexture surfaceTexture) {
        logInfo("onSurfaceCreated " + surfaceTexture);
        createEglSurfaceInternal(surfaceTexture);
    }

    public void onSurfaceChanged(int width, int height) {
        logInfo("onSurfaceChanged " + width + "x" + height);
        mRenderHandler.post(() -> {
            if (mNativeHandle != 0) {
                nativeOnSurfaceChanged(mNativeHandle, width, height);
            }
        });
    }

    public void onSurfaceDestroyed() {
        logInfo("onSurfaceDestroyed");
        mEglSurfaceCreationRunnable.setSurface(null);
        mRenderHandler.removeCallbacks(mEglSurfaceCreationRunnable);
        ThreadUtils.invokeAtFrontUninterruptibly(mRenderHandler, () -> {
            if (mNativeHandle != 0) {
                nativeOnSurfaceDestroyed(mNativeHandle);
            }
            if (mEglBase != null) {
                mEglBase.release();
                mEglBase = null;
            }
        });
        logInfo("onSurfaceDestroyed success");
    }

    public void renderFrame(int textureId, int width, int height, long timestamp) {
        mRenderHandler.post(() -> {
            if (mNativeHandle != 0) {
                nativeRenderFrame(mNativeHandle, textureId, width, height, timestamp);
                mEglBase.swapBuffers(timestamp);
            }
        });
    }

    public void destroy() {
        logInfo("destroy");
        mRenderHandler.post(() -> {
            if (mNativeHandle != 0) {
                nativeDestroy(mNativeHandle);
            }
            mNativeHandle = 0;
            mRenderHandler.getLooper().quit();
        });
    }

    private void createEglSurfaceInternal(Object surface) {
        mEglSurfaceCreationRunnable.setSurface(surface);
        mRenderHandler.post(mEglSurfaceCreationRunnable);
    }

    private void logInfo(String content) {
        Logging.info(TAG, mName + "(" + hashCode() + ") " + content);
    }

    private void logError(String content) {
        Logging.error(TAG, mName + "(" + hashCode() + ") " + content);
    }

    private class EglSurfaceCreation implements Runnable {
        private Object mSurface;

        synchronized void setSurface(Object surface) {
            this.mSurface = surface;
        }

        @Override
        public synchronized void run() {
            if (mSurface != null && mEglBase != null && !mEglBase.hasSurface()) {
                if (mSurface instanceof Surface) {
                    mEglBase.createSurface((Surface) mSurface);
                } else if (mSurface instanceof SurfaceTexture) {
                    mEglBase.createSurface((SurfaceTexture) mSurface);
                } else {
                    logError("EglSurfaceCreation error: invalid surface " + mSurface);
                    return;
                }
                mEglBase.makeCurrent();
                // Necessary for YUV frames with odd width.
                GLES20.glPixelStorei(GLES20.GL_UNPACK_ALIGNMENT, 1);
            }
        }
    }
}
