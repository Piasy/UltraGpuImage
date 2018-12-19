#include <jni.h>

#include "ugi_renderer.h"

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_piasy_ugi_UgiRenderer_nativeCreate(JNIEnv* env, jclass type) {
    UgiRenderer* renderer = new UgiRenderer();
    return reinterpret_cast<jlong>(renderer);
}

JNIEXPORT void JNICALL
Java_com_piasy_ugi_UgiRenderer_nativeOnSurfaceChanged(
        JNIEnv* env, jclass type, jlong handle, jint width, jint height) {
    UgiRenderer* renderer = reinterpret_cast<UgiRenderer*>(handle);
    if (renderer) {
        renderer->OnSurfaceChanged(width, height);
    }
}

JNIEXPORT void JNICALL
Java_com_piasy_ugi_UgiRenderer_nativeOnSurfaceDestroyed(JNIEnv* env, jclass type, jlong handle) {
    UgiRenderer* renderer = reinterpret_cast<UgiRenderer*>(handle);
    if (renderer) {
        renderer->OnSurfaceDestroyed();
    }
}

JNIEXPORT void JNICALL
Java_com_piasy_ugi_UgiRenderer_nativeRenderFrame(
        JNIEnv* env, jclass type, jlong handle, jint textureId, jint width, jint height,
        jlong timestamp) {
    UgiRenderer* renderer = reinterpret_cast<UgiRenderer*>(handle);
    if (renderer) {
        renderer->RenderFrame(static_cast<GLuint>(textureId), width, height, timestamp);
    }
}

JNIEXPORT void JNICALL
Java_com_piasy_ugi_UgiRenderer_nativeDestroy(JNIEnv* env, jclass type, jlong handle) {
    UgiRenderer* renderer = reinterpret_cast<UgiRenderer*>(handle);
    if (renderer) {
        delete renderer;
    }
}

}
