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
#if defined(__ANDROID__)
    kTextureTypeOes,
#endif
    kTextureTypeMax,
};

enum FilterType {
    kFilterGroup = 0,
    kFilterPreprocess = 1,
    kFilterColorInvert = 2,
    kFilterBrightness = 3,
};

class Filter {
public:
    Filter();

    virtual ~Filter();

    Filter(const GLchar* vertex_shader, const GLchar* fragment_shader);

    static const GLchar* kDirectVertexShader;
    static const GLchar* kDirectFragmentShader;

    virtual int Init(int32_t output_width, int32_t output_height);

    virtual int Apply(TextureType type, GLuint texture_id, GLuint vao);

    virtual int Destroy();

    virtual bool IsGroup();

protected:
    GLuint CreateProgram(const GLchar* vertex_shader, const GLchar* fragment_shader);

    virtual void PreDraw();

    int DoApply(GLuint program, TextureType type, GLuint texture_id, GLuint vao);

    GLuint program_;

private:
    GLenum textureType(TextureType type);

    const GLchar* vertex_shader_;
    const GLchar* fragment_shader_;
};

}

#endif //ULTRAGPUIMAGE_UGI_FILTER_H
