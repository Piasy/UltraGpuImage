package com.piasy.ugi.example;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.hardware.Camera;
import android.os.Bundle;
import android.widget.CheckBox;
import androidx.annotation.NonNull;
import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;
import com.piasy.ugi.UgiTextureView;
import permissions.dispatcher.NeedsPermission;
import permissions.dispatcher.RuntimePermissions;

@RuntimePermissions
public class SelectorActivity extends Activity {

    @BindView(R.id.frontCamera)
    CheckBox frontCamera;
    @BindView(R.id.portrait)
    CheckBox portrait;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_selector);

        SelectorActivityPermissionsDispatcher.checkPermissionWithPermissionCheck(this);
    }

    @Override
    public void onRequestPermissionsResult(final int requestCode,
            @NonNull final String[] permissions, @NonNull final int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        SelectorActivityPermissionsDispatcher.onRequestPermissionsResult(this, requestCode,
                grantResults);
    }

    @OnClick(R.id.picture)
    void picture() {
        startDemo(UgiTextureView.RENDER_MODE_PICTURE);
    }

    @OnClick(R.id.camera)
    void camera() {
        startDemo(UgiTextureView.RENDER_MODE_CAMERA_PREVIEW);
    }

    @NeedsPermission({
            Manifest.permission.CAMERA,
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
    })
    void checkPermission() {
        ButterKnife.bind(this);
    }

    private void startDemo(int renderMode) {
        Intent intent = new Intent(this,
                portrait.isChecked() ? MainActivity.class : LandscapePreviewActivity.class);
        intent.putExtra(MainActivity.MODE_EXTRA_KEY, renderMode);
        intent.putExtra(MainActivity.CAMERA_FACE_EXTRA_KEY,
                frontCamera.isChecked() ? Camera.CameraInfo.CAMERA_FACING_FRONT
                        : Camera.CameraInfo.CAMERA_FACING_BACK);
        startActivity(intent);
    }
}
