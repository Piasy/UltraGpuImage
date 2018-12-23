#include <jni.h>
#include <android/log.h>

#include "ugi_renderer.h"
#include "ugi_transformation.h"
#include "ugi_logging.h"

extern "C" {

static bool g_log_func_set_ = false;

static void ugi_android_log(int level, const char* log) {
    if (level == UGI_LOG_LEVEL_ERROR) {
        __android_log_write(ANDROID_LOG_ERROR, "ugi", log);
    } else {
        __android_log_write(ANDROID_LOG_INFO, "ugi", log);
    }
}

JNIEXPORT jlong JNICALL
Java_com_piasy_ugi_UgiRenderer_nativeCreate(JNIEnv* env, jclass type, jlong nativeTransformation) {
    if (!g_log_func_set_) {
        g_log_func_set_ = true;
        ugi_set_log_func(ugi_android_log);
    }

    Ugi::Transformation* transformation
            = reinterpret_cast<Ugi::Transformation*>(nativeTransformation);
    if (transformation) {
        Ugi::Renderer* renderer = new Ugi::Renderer(*transformation);
        return reinterpret_cast<jlong>(renderer);
    } else {
        return 0;
    }
}

JNIEXPORT void JNICALL
Java_com_piasy_ugi_UgiRenderer_nativeOnSurfaceCreated(
        JNIEnv* env, jclass type, jlong handle) {
    Ugi::Renderer* renderer = reinterpret_cast<Ugi::Renderer*>(handle);
    if (renderer) {
        renderer->OnSurfaceCreated();
    }
}

JNIEXPORT void JNICALL
Java_com_piasy_ugi_UgiRenderer_nativeOnSurfaceDestroyed(JNIEnv* env, jclass type, jlong handle) {
    Ugi::Renderer* renderer = reinterpret_cast<Ugi::Renderer*>(handle);
    if (renderer) {
        renderer->OnSurfaceDestroyed();
    }
}

JNIEXPORT void JNICALL
Java_com_piasy_ugi_UgiRenderer_nativeRenderTexture(
        JNIEnv* env, jclass type, jlong handle, jint textureType, jint textureId) {
    Ugi::Renderer* renderer = reinterpret_cast<Ugi::Renderer*>(handle);
    if (renderer) {
        renderer->RenderTexture(Ugi::TextureType(textureType), static_cast<GLuint>(textureId));
    }
}

JNIEXPORT void JNICALL
Java_com_piasy_ugi_UgiRenderer_nativeUpdateTransformation(
        JNIEnv* env, jclass type, jlong handle, jlong nativeTransformation) {
    Ugi::Renderer* renderer = reinterpret_cast<Ugi::Renderer*>(handle);
    Ugi::Transformation* transformation
            = reinterpret_cast<Ugi::Transformation*>(nativeTransformation);
    if (renderer && transformation) {
        renderer->UpdateTransformation(*transformation);
    }
}

JNIEXPORT void JNICALL
Java_com_piasy_ugi_UgiRenderer_nativeSetFilter(
        JNIEnv* env, jclass type, jlong handle, jlong filterHandle) {
    Ugi::Renderer* renderer = reinterpret_cast<Ugi::Renderer*>(handle);
    Ugi::Filter* filter = reinterpret_cast<Ugi::Filter*>(filterHandle);
    if (renderer && filter) {
        renderer->SetFilter(filter);
    }
}

JNIEXPORT void JNICALL
Java_com_piasy_ugi_UgiRenderer_nativeDestroy(JNIEnv* env, jclass type, jlong handle) {
    Ugi::Renderer* renderer = reinterpret_cast<Ugi::Renderer*>(handle);
    if (renderer) {
        delete renderer;
    }
}

JNIEXPORT jlong JNICALL
Java_com_piasy_ugi_UgiTransformation_nativeCreate(
        JNIEnv* env, jclass type, jint inputWidth, jint inputHeight, jint outputWidth,
        jint outputHeight) {
    Ugi::Transformation* transformation
            = new Ugi::Transformation(Ugi::Transformation::Size(inputWidth, inputHeight),
                                      Ugi::Transformation::Size(outputWidth, outputHeight));
    return reinterpret_cast<jlong>(transformation);
}

JNIEXPORT void JNICALL
Java_com_piasy_ugi_UgiTransformation_nativeUpdateInput(
        JNIEnv* env, jclass type, jlong handle, jint width, jint height) {
    Ugi::Transformation* transformation = reinterpret_cast<Ugi::Transformation*>(handle);
    if (transformation) {
        transformation->UpdateInput(Ugi::Transformation::Size(width, height));
    }
}

JNIEXPORT void JNICALL
Java_com_piasy_ugi_UgiTransformation_nativeUpdateOutput(
        JNIEnv* env, jclass type, jlong handle, jint width, jint height) {
    Ugi::Transformation* transformation = reinterpret_cast<Ugi::Transformation*>(handle);
    if (transformation) {
        transformation->UpdateOutput(Ugi::Transformation::Size(width, height));
    }
}

JNIEXPORT void JNICALL
Java_com_piasy_ugi_UgiTransformation_nativeUpdateCrop(
        JNIEnv* env, jclass type, jlong handle, jint x, jint y, jint width, jint height) {
    Ugi::Transformation* transformation = reinterpret_cast<Ugi::Transformation*>(handle);
    if (transformation) {
        transformation->UpdateCrop(Ugi::Transformation::Rect(x, y, width, height));
    }
}

JNIEXPORT void JNICALL
Java_com_piasy_ugi_UgiTransformation_nativeUpdateFlip(
        JNIEnv* env, jclass type, jlong handle, jint flip) {
    Ugi::Transformation* transformation = reinterpret_cast<Ugi::Transformation*>(handle);
    if (transformation) {
        transformation->UpdateFlip(Ugi::Transformation::Flip(flip));
    }
}

JNIEXPORT void JNICALL
Java_com_piasy_ugi_UgiTransformation_nativeUpdateRotation(
        JNIEnv* env, jclass type, jlong handle, jint rotation) {
    Ugi::Transformation* transformation = reinterpret_cast<Ugi::Transformation*>(handle);
    if (transformation) {
        transformation->UpdateRotation(Ugi::Transformation::Rotation(rotation));
    }
}

JNIEXPORT void JNICALL
Java_com_piasy_ugi_UgiTransformation_nativeUpdateScaleType(
        JNIEnv* env, jclass type, jlong handle, jint scaleType) {
    Ugi::Transformation* transformation = reinterpret_cast<Ugi::Transformation*>(handle);
    if (transformation) {
        transformation->UpdateScaleType(Ugi::Transformation::ScaleType(scaleType));
    }
}

JNIEXPORT jint JNICALL
Java_com_piasy_ugi_UgiTransformation_nativeGetInputWidth(JNIEnv* env, jclass type, jlong handle) {
    Ugi::Transformation* transformation = reinterpret_cast<Ugi::Transformation*>(handle);
    return transformation ? transformation->input_width() : 0;
}

JNIEXPORT jint JNICALL
Java_com_piasy_ugi_UgiTransformation_nativeGetInputHeight(JNIEnv* env, jclass type, jlong handle) {
    Ugi::Transformation* transformation = reinterpret_cast<Ugi::Transformation*>(handle);
    return transformation ? transformation->input_height() : 0;
}

JNIEXPORT jint JNICALL
Java_com_piasy_ugi_UgiTransformation_nativeGetRotation(JNIEnv* env, jclass type, jlong handle) {
    Ugi::Transformation* transformation = reinterpret_cast<Ugi::Transformation*>(handle);
    return transformation ? transformation->rotation() : 0;
}

JNIEXPORT jint JNICALL
Java_com_piasy_ugi_UgiTransformation_nativeGetFlip(JNIEnv* env, jclass type, jlong handle) {
    Ugi::Transformation* transformation = reinterpret_cast<Ugi::Transformation*>(handle);
    return transformation ? transformation->flip() : 0;
}

JNIEXPORT void JNICALL
Java_com_piasy_ugi_UgiTransformation_nativeDestroy(JNIEnv* env, jclass type, jlong handle) {
    Ugi::Transformation* transformation = reinterpret_cast<Ugi::Transformation*>(handle);
    if (transformation) {
        delete transformation;
    }
}

}
