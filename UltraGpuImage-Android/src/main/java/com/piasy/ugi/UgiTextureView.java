package com.piasy.ugi;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Matrix;
import android.graphics.SurfaceTexture;
import android.opengl.EGLContext;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.util.AttributeSet;
import android.view.TextureView;
import androidx.annotation.IntDef;
import androidx.annotation.NonNull;
import com.piasy.ugi.egl.EglBase;

/**
 * Created by Piasy{github.com/Piasy} on 2018/12/21.
 */
public class UgiTextureView extends TextureView implements TextureView.SurfaceTextureListener {
    public static final int RENDER_MODE_PICTURE = 1;
    public static final int RENDER_MODE_CAMERA_PREVIEW = 2;

    private int mRenderMode;
    private UgiRenderer mRenderer;
    private SurfaceTexture mSurfaceTexture;

    private int mTextureWidth;
    private int mTextureHeight;

    private Bitmap mPicture;
    private boolean mRecycleAfterUpload;
    private int mPictureTextureId = -1;
    private int mPictureWidth;
    private int mPictureHeight;

    private TextureView.SurfaceTextureListener mSurfaceTextureListener;

    public UgiTextureView(final Context context) {
        this(context, null);
    }

    public UgiTextureView(final Context context, final AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public UgiTextureView(final Context context, final AttributeSet attrs, final int defStyleAttr) {
        super(context, attrs, defStyleAttr);

        super.setSurfaceTextureListener(this);
    }

    private static int loadTexture(Bitmap bitmap, boolean recycle) {
        int textures[] = new int[1];
        GLES20.glGenTextures(1, textures, 0);
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textures[0]);
        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
                GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
                GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
                GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
                GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
        GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap, 0);

        if (recycle) {
            bitmap.recycle();
        }

        return textures[0];
    }

    private static void destroyTexture(int textureId) {
        int textures[] = new int[] { textureId };
        GLES20.glDeleteTextures(1, textures, 0);
    }

    public synchronized void init(EGLContext sharedContext, @RenderMode int renderMode) {
        mRenderMode = renderMode;
        mRenderer = new UgiRenderer(sharedContext, EglBase.CONFIG_PLAIN);
        tryCreateRendererSurface();
    }

    public synchronized void destroy() {
        if (mRenderer != null) {
            if (mPictureTextureId != -1) {
                mRenderer.runOnRenderThread(() -> {
                    destroyTexture(mPictureTextureId);
                    mPictureTextureId = -1;
                });
            }
            if (mPicture != null && mRecycleAfterUpload) {
                mPicture.recycle();
                mPicture = null;
            }
            mRenderer.destroy();
            mRenderer = null;
        }
    }

    public synchronized void updateCameraInfo(int width, int height, int sensorDegree,
            boolean frontFace, boolean isLandscape) {
        if (mRenderer != null) {
            UgiTransformation transformation = mRenderer.getTransformation();
            if (isLandscape) {
                transformation.updateRotation(360 - sensorDegree + 90);
                transformation.updateInput(width, height);
            } else {
                transformation.updateRotation(360 - sensorDegree);
                transformation.updateInput(height, width);
            }
            if (!frontFace && sensorDegree % 180 != 0) {
                transformation.updateFlip(UgiTransformation.FLIP_VERTICAL);
            } else {
                transformation.updateFlip(UgiTransformation.FLIP_NONE);
            }
            notifyTransformationUpdated();
        }
    }

    public synchronized UgiTransformation getTransformation() {
        return mRenderer == null ? null : mRenderer.getTransformation();
    }

    public synchronized void notifyTransformationUpdated() {
        if (mRenderer != null && mSurfaceTexture != null) {
            mRenderer.notifyTransformationUpdated();

            if (mRenderMode == RENDER_MODE_PICTURE && mPictureTextureId != -1) {
                mRenderer.renderRgb(mPictureTextureId, 0);
            }
        }
    }

    public synchronized void notifyFilterUpdated() {
        if (mRenderer != null && mSurfaceTexture != null
            && mRenderMode == RENDER_MODE_PICTURE && mPictureTextureId != -1) {
            mRenderer.renderRgb(mPictureTextureId, 0);
        }
    }

    public synchronized void setFilter(UgiFilter filter) {
        if (mRenderer != null) {
            mRenderer.setFilter(filter);
            notifyFilterUpdated();
        }
    }

    public synchronized void renderOes(int textureId) {
        if (mRenderer != null && mSurfaceTexture != null) {
            mRenderer.renderOes(textureId, 0);
        }
    }

    public void renderPicture(@NonNull Bitmap bitmap) {
        renderPicture(bitmap, true, true);
    }

    public synchronized void renderPicture(@NonNull Bitmap bitmap, boolean needFlipVertically,
            boolean recycle) {
        if (mRenderMode == RENDER_MODE_PICTURE && mRenderer != null && !bitmap.isRecycled()) {
            mPictureWidth = bitmap.getWidth();
            mPictureHeight = bitmap.getHeight();
            if (needFlipVertically) {
                Matrix matrix = new Matrix();
                matrix.postScale(1, -1, mPictureWidth / 2F, mPictureHeight / 2F);
                mPicture = Bitmap.createBitmap(bitmap, 0, 0, mPictureWidth, mPictureHeight,
                        matrix, true);
                if (recycle) {
                    bitmap.recycle();
                }
            } else {
                mPicture = bitmap;
            }
            mRecycleAfterUpload = recycle;
            tryUploadPicture();
        }
    }

    private void tryCreateRendererSurface() {
        if (mRenderer != null && mSurfaceTexture != null) {
            mRenderer.onSurfaceCreated(mSurfaceTexture);
            tryUploadPicture();
        }
    }

    private void tryUploadPicture() {
        mRenderer.runOnRenderThread(() -> {
            synchronized (UgiTextureView.this) {
                if (mPicture != null && mSurfaceTexture != null) {
                    if (mPictureTextureId != -1) {
                        destroyTexture(mPictureTextureId);
                    }
                    mPictureTextureId = loadTexture(mPicture, mRecycleAfterUpload);
                    mPicture = null;

                    updateInputOutputSize();
                }
            }
        });
    }

    private void updateInputOutputSize() {
        if (mRenderer != null) {
            UgiTransformation transformation = mRenderer.getTransformation();
            transformation.updateOutput(mPictureWidth, mPictureHeight);
            transformation.updateOutput(mTextureWidth, mTextureHeight);
            notifyTransformationUpdated();
        }
    }

    @Override
    public synchronized void setSurfaceTextureListener(
            final SurfaceTextureListener surfaceTextureListener) {
        mSurfaceTextureListener = surfaceTextureListener;
    }

    @Override
    public synchronized void onSurfaceTextureAvailable(final SurfaceTexture surface,
            final int width, final int height) {
        mSurfaceTexture = surface;
        mTextureWidth = width;
        mTextureHeight = height;
        tryCreateRendererSurface();

        if (mSurfaceTextureListener != null) {
            mSurfaceTextureListener.onSurfaceTextureAvailable(surface, width, height);
        }
    }

    @Override
    public synchronized void onSurfaceTextureSizeChanged(final SurfaceTexture surface,
            final int width, final int height) {
        mTextureWidth = width;
        mTextureHeight = height;
        updateInputOutputSize();

        if (mSurfaceTextureListener != null) {
            mSurfaceTextureListener.onSurfaceTextureSizeChanged(surface, width, height);
        }
    }

    @Override
    public synchronized boolean onSurfaceTextureDestroyed(final SurfaceTexture surface) {
        if (mRenderer != null) {
            mRenderer.onSurfaceDestroyed();
        }
        mSurfaceTexture = null;

        if (mSurfaceTextureListener != null) {
            mSurfaceTextureListener.onSurfaceTextureDestroyed(surface);
        }

        return true;
    }

    @Override
    public void onSurfaceTextureUpdated(final SurfaceTexture surface) {
        if (mSurfaceTextureListener != null) {
            mSurfaceTextureListener.onSurfaceTextureUpdated(surface);
        }
    }

    @IntDef(value = {
            RENDER_MODE_PICTURE,
            RENDER_MODE_CAMERA_PREVIEW,
    })
    @interface RenderMode {
    }
}
