#include <jni.h>

#include "filters/ugi_filter_group.h"
#include "filters/ugi_preprocess_filter.h"
#include "filters/ugi_color_invert_filter.h"
#include "filters/ugi_brightness_filter.h"

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_piasy_ugi_UgiFilter_nativeCreate(JNIEnv* env, jclass type, jint filterType) {
    Ugi::Filter* filter = nullptr;
    switch (Ugi::FilterType(filterType)) {
        case Ugi::kFilterGroup:
            filter = new Ugi::FilterGroup();
            break;
        case Ugi::kFilterPreprocess:
            filter = new Ugi::PreprocessFilter();
            break;
        case Ugi::kFilterColorInvert:
            filter = new Ugi::ColorInvertFilter();
            break;
        case Ugi::kFilterBrightness:
            filter = new Ugi::BrightnessFilter();
            break;
        default:
            break;
    }
    return reinterpret_cast<jlong>(filter);
}

JNIEXPORT void JNICALL
Java_com_piasy_ugi_filters_UgiFilterGroup_nativeAddFilter(
        JNIEnv* env, jclass type, jlong handle, jlong filterHandle) {
    Ugi::FilterGroup* group = reinterpret_cast<Ugi::FilterGroup*>(handle);
    Ugi::Filter* filter = reinterpret_cast<Ugi::Filter*>(filterHandle);
    if (group && filter) {
        group->AddFilter(filter);
    }
}

JNIEXPORT void JNICALL
Java_com_piasy_ugi_filters_UgiBrightnessFilter_nativeSetBrightness(
        JNIEnv* env, jclass type, jlong handle, jfloat brightness) {
    Ugi::BrightnessFilter* filter = reinterpret_cast<Ugi::BrightnessFilter*>(handle);
    if (filter) {
        filter->SetBrightness(brightness);
    }
}

}
