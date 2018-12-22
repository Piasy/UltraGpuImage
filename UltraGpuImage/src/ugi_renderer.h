//
// Created by Piasy on 2018/12/17.
//

#ifndef ULTRAGPUIMAGE_UGI_RENDERER_H
#define ULTRAGPUIMAGE_UGI_RENDERER_H

#include <stdint.h>
#include <memory>

#include "ugi_gl.h"
#include "ugi_transformation.h"
#include "ugi_filter.h"

namespace Ugi {

class Renderer {
public:
    Renderer(Transformation transformation);

    ~Renderer();

    void OnSurfaceCreated();

    void OnSurfaceDestroyed();

    void UpdateTransformation(Transformation transformation);

    void RenderTexture(TextureType type, GLuint texture_id);

    void SetFilter(Filter* filter);

private:
    void bindBuffers();

    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;

    Transformation transformation_;
    GLfloat vertex_attributes_[16];

    std::unique_ptr<Filter> filter_;
};

}

#endif //ULTRAGPUIMAGE_UGI_RENDERER_H
