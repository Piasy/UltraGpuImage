//
// Created by Piasy on 2018/12/22.
//

#include "ugi_preprocess_filter.h"

namespace Ugi {

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

int PreprocessFilter::Init(int32_t output_width, int32_t output_height) {
    int result = Filter::Init(output_width, output_height);
    if (result != 0) {
        return result;
    }

    oes_program_ = CreateProgram(kDirectVertexShader, kFragmentShaderOes);
    if (oes_program_ == 0) {
        return -1;
    }
    return 0;
}

int PreprocessFilter::Apply(TextureType type, GLuint texture_id, GLuint vao) {
    switch (type) {
        case kTextureTypeOes:
            return DoApply(oes_program_, type, texture_id, vao);
        case kTextureTypeRgb:
        default:
            return DoApply(program_, type, texture_id, vao);
    }
}

int PreprocessFilter::Destroy() {
    if (oes_program_ != 0) {
        glDeleteProgram(oes_program_);
        oes_program_ = 0;
    }

    return Filter::Destroy();
}

}
