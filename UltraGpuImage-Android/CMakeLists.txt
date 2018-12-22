cmake_minimum_required(VERSION 3.4.1)

include_directories(
        ../UltraGpuImage/src
)

file(GLOB_RECURSE ugi_filter_sources
        ../UltraGpuImage/src/filters/*.cpp
        )

file(GLOB_RECURSE ugi_sources
        ../UltraGpuImage/src/ugi_logging.cpp
        ../UltraGpuImage/src/ugi_renderer.cpp
        ../UltraGpuImage/src/ugi_transformation.cpp
        ../UltraGpuImage/src/ugi_filter.cpp
        )

add_library(
        UltraGpuImage

        SHARED

        ${ugi_sources}
        ${ugi_filter_sources}
        src/main/cpp/ultra_gpu_image_jni.cpp
)


target_link_libraries(
        UltraGpuImage

        log
        EGL
        GLESv3
)
