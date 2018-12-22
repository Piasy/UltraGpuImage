//
// Created by Piasy on 2018/12/17.
//

#ifndef ULTRAGPUIMAGE_UGI_RENDERER_H
#define ULTRAGPUIMAGE_UGI_RENDERER_H

#include <stdint.h>

#include "ugi_gl.h"
#include "ugi_transformation.h"

namespace Ugi {

class Renderer {
public:
    Renderer(Transformation transformation);

    ~Renderer();

    void OnSurfaceCreated();

    void OnSurfaceDestroyed();

    void UpdateTransformation(Transformation transformation);

    void RenderRgb(GLuint texture_id, int64_t timestamp);

    void RenderOes(GLuint texture_id, int64_t timestamp);

private:
    enum TextureType {
        kTextureTypeRgb,
        kTextureTypeOes,
        kTextureTypeMax,
    };

    void prepareShader(TextureType type);

    GLenum textureType(TextureType type);

    void bindBuffers();

    void renderTexture(TextureType type, GLuint texture_id, int64_t timestamp);

    GLuint programs_[kTextureTypeMax];
    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;

    Transformation transformation_;
    GLfloat vertex_attributes_[16];
};

}

#endif //ULTRAGPUIMAGE_UGI_RENDERER_H
