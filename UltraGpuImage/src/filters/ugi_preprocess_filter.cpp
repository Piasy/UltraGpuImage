//
// Created by Piasy on 2018/12/22.
//

#include "ugi_preprocess_filter.h"

namespace Ugi {

#if defined(__ANDROID__)
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
#endif

int PreprocessFilter::Init(int32_t output_width, int32_t output_height) {
    int result = Filter::Init(output_width, output_height);
    if (result != 0) {
        return result;
    }

#if defined(__ANDROID__)
    oes_program_ = CreateProgram(kDirectVertexShader, kFragmentShaderOes);
    if (oes_program_ == 0) {
        return -1;
    }
#endif
    return 0;
}

int PreprocessFilter::Apply(TextureType type, GLuint texture_id, GLuint vao) {
    switch (type) {
#if defined(__ANDROID__)
        case kTextureTypeOes:
            return DoApply(oes_program_, type, texture_id, vao);
#endif
        case kTextureTypeRgb:
        default:
            return DoApply(program_, type, texture_id, vao);
    }
}

int PreprocessFilter::Destroy() {
#if defined(__ANDROID__)
    if (oes_program_ != 0) {
        glDeleteProgram(oes_program_);
        oes_program_ = 0;
    }
#endif

    return Filter::Destroy();
}

}
