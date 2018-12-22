//
// Created by Piasy on 2018/12/22.
//

#ifndef ULTRAGPUIMAGE_UGI_FILTER_H
#define ULTRAGPUIMAGE_UGI_FILTER_H

#include "ugi_gl.h"
#include "ugi_transformation.h"

namespace Ugi {

#define UGI_GLSL_VERSION "300 es"

enum TextureType {
    kTextureTypeRgb,
    kTextureTypeOes,
    kTextureTypeMax,
};

class Filter {
public:
    Filter(const GLchar* vertex_shader, const GLchar* fragment_shader);

    virtual int Init();

    virtual int Apply(TextureType type, GLuint texture_id, GLuint vao);

    virtual int Destroy();

protected:
    GLuint CreateProgram(const GLchar* vertex_shader, const GLchar* fragment_shader);

    int DoApply(GLuint program, TextureType type, GLuint texture_id, GLuint vao);

    GLuint program_;

private:
    GLenum textureType(TextureType type);

    const GLchar* vertex_shader_;
    const GLchar* fragment_shader_;
};

}

#endif //ULTRAGPUIMAGE_UGI_FILTER_H
