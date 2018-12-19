package com.piasy.ultragpuimage.example;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.graphics.SurfaceTexture;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.os.Bundle;
import android.view.TextureView;
import com.piasy.ugi.UgiRenderer;
import com.piasy.ugi.egl.EglBase;
import com.piasy.ugi.utils.Logging;

public class MainActivity extends Activity {

    private volatile boolean mRunning;
    private UgiRenderer mRenderer;
    private volatile int mTexture = -1;
    private int mTextureWidth;
    private int mTextureHeight;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Logging.setLogToConsole(true);

        TextureView surface = findViewById(R.id.surface);
        mRenderer = new UgiRenderer(null, EglBase.CONFIG_PLAIN);

        surface.setSurfaceTextureListener(new TextureView.SurfaceTextureListener() {
            @Override
            public void onSurfaceTextureAvailable(final SurfaceTexture surface, final int width,
                    final int height) {
                mRenderer.onSurfaceCreated(surface);
                mRenderer.onSurfaceChanged(width, height);
                startRender();

                mRenderer.runOnRenderThread(() -> {
                    Bitmap bitmap = BitmapFactory.decodeResource(getResources(),
                            R.drawable.awesomeface);
                    mTextureWidth = bitmap.getWidth();
                    mTextureHeight = bitmap.getHeight();
                    Matrix matrix = new Matrix();
                    matrix.postScale(1, -1, mTextureWidth / 2F, mTextureHeight / 2F);
                    mTexture = loadTexture(
                            Bitmap.createBitmap(bitmap, 0, 0, mTextureWidth, mTextureHeight, matrix,
                                    true));
                });
            }

            @Override
            public void onSurfaceTextureSizeChanged(final SurfaceTexture surface, final int width,
                    final int height) {
                mRenderer.onSurfaceChanged(width, height);
            }

            @Override
            public boolean onSurfaceTextureDestroyed(final SurfaceTexture surface) {
                mRenderer.onSurfaceDestroyed();
                return true;
            }

            @Override
            public void onSurfaceTextureUpdated(final SurfaceTexture surface) {
            }
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        mRunning = false;
    }

    private int loadTexture(Bitmap bitmap) {
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

        bitmap.recycle();

        return textures[0];
    }

    private void startRender() {
        new Thread(() -> {
            mRunning = true;
            while (mRunning) {
                if (mTexture != -1) {
                    mRenderer.renderRgb(mTexture, mTextureWidth, mTextureHeight, 0);
                }
                try {
                    Thread.sleep(40);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            mRenderer.destroy();
        }).start();
    }
}
