//
// Created by Piasy on 2018/12/22.
//

#include "ugi_preprocess_filter.h"

namespace Ugi {

static const GLchar* kVertexShader
        = "#version " UGI_GLSL_VERSION "\n"
          "layout(location = 0) in vec2 aVertCoords;\n"
          "layout(location = 1) in vec2 aTexCoords;\n"
          "out vec2 texCoords;\n"
          "void main() {\n"
          "    gl_Position = vec4(aVertCoords, 0.0, 1.0);\n"
          "    texCoords = aTexCoords;\n"
          "}\n";
static const GLchar* kFragmentShaderRgb
        = "#version " UGI_GLSL_VERSION "\n"
          "#ifdef GL_ES\n"
          "precision mediump float;\n"
          "#endif\n"
          "in vec2 texCoords;\n"
          "uniform sampler2D tex;\n"
          "out vec4 fragColor;\n"
          "void main() {\n"
          "    fragColor = texture(tex, texCoords);\n"
          "}\n";
static const GLchar* kFragmentShaderOes
        = "#version " UGI_GLSL_VERSION "\n"
          "#extension GL_OES_EGL_image_external_essl3: require\n"
          "#ifdef GL_ES\n"
          "precision mediump float;\n"
          "#endif\n"
          "in vec2 texCoords;\n"
          "uniform samplerExternalOES tex;\n"
          "out vec4 fragColor;\n"
          "void main() {\n"
          "    fragColor = texture(tex, texCoords);\n"
          "}\n";

PreprocessFilter::PreprocessFilter() : Filter(kVertexShader, kFragmentShaderRgb) {}

int PreprocessFilter::Apply(TextureType type, GLuint texture_id, GLuint vao) {
    switch (type) {
        case kTextureTypeOes:
            return DoApply(oes_program_, type, texture_id, vao);
        case kTextureTypeRgb:
        default:
            return DoApply(program_, type, texture_id, vao);
    }
}

int PreprocessFilter::Init() {
    int result = Filter::Init();
    if (result != 0) {
        return result;
    }

    oes_program_ = CreateProgram(kVertexShader, kFragmentShaderOes);
    if (oes_program_ == 0) {
        return -1;
    }
    return 0;
}

int PreprocessFilter::Destroy() {
    if (oes_program_ != 0) {
        glDeleteProgram(oes_program_);
        oes_program_ = 0;
    }

    return Filter::Destroy();
}

}
