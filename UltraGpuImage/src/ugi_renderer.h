//
// Created by Piasy on 2018/12/17.
//

#ifndef ULTRAGPUIMAGE_UGI_RENDERER_H
#define ULTRAGPUIMAGE_UGI_RENDERER_H

#include <stdint.h>

#include "ugi_gl.h"

class UgiRenderer {
public:
    UgiRenderer();

    ~UgiRenderer();

    void OnSurfaceChanged(int32_t width, int32_t height);

    void OnSurfaceDestroyed();

    void RenderFrame(GLuint texture_id, int32_t width, int32_t height, int64_t timestamp);

private:
    GLuint program_;
    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;
};


#endif //ULTRAGPUIMAGE_UGI_RENDERER_H
