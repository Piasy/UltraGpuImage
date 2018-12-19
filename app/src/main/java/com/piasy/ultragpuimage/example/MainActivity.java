package com.piasy.ultragpuimage.example;

import android.app.Activity;
import android.graphics.SurfaceTexture;
import android.os.Bundle;
import android.view.TextureView;
import com.piasy.ugi.UgiRenderer;
import com.piasy.ugi.egl.EglBase;
import com.piasy.ugi.utils.Logging;

public class MainActivity extends Activity {

    private volatile boolean mRunning;
    private UgiRenderer mRenderer;

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

    private void startRender() {
        new Thread(() -> {
            mRunning = true;
            while (mRunning) {
                mRenderer.renderFrame(0, 0, 0, 0);
                try {
                    Thread.sleep(40);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            mRenderer.destroy();
        }).start();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        mRunning = false;
    }
}
