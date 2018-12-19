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

    void RenderRgb(GLuint texture_id, int32_t width, int32_t height, int64_t timestamp);

    void RenderOes(GLuint texture_id, int32_t width, int32_t height, int64_t timestamp);

private:
    enum TextureType {
        kTextureTypeRgb,
        kTextureTypeOes,
        kTextureTypeMax,
    };

    void prepareShader(TextureType type);

    GLenum textureType(TextureType type);

    void renderTexture(TextureType type, GLuint texture_id, int32_t width, int32_t height,
                       int64_t timestamp);

    GLuint programs_[kTextureTypeMax];
    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;
};


#endif //ULTRAGPUIMAGE_UGI_RENDERER_H
