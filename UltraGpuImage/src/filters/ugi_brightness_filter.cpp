//
// Created by Piasy on 2018/12/23.
//

#include "ugi_brightness_filter.h"

namespace Ugi {

static const GLchar* kFragmentShader
        = "#version " UGI_GLSL_VERSION "\n"
          "#ifdef GL_ES\n"
          "precision mediump float;\n"
          "#endif\n"
          "in vec2 texCoords;\n"
          "uniform sampler2D tex;\n"
          "uniform float brightness;\n"
          "out vec4 fragColor;\n"
          "void main() {\n"
          "    vec4 color = texture(tex, texCoords);\n"
          "    fragColor = vec4((color.rgb + vec3(brightness)), color.a);\n"
          "}\n";

BrightnessFilter::BrightnessFilter() : Filter(kDirectVertexShader, kFragmentShader),
                                       brightness_(0) {

}

int BrightnessFilter::Init(int32_t width, int32_t height) {
    int result = Filter::Init(width, height);

    if (result == 0) {
        brightness_loc_ = glGetUniformLocation(program_, "brightness");
    }

    return result;
}

void BrightnessFilter::PreDraw() {
    Filter::PreDraw();

    glUniform1f(brightness_loc_, brightness_);
}

void BrightnessFilter::SetBrightness(GLfloat brightness) {
    brightness_ = brightness;
}

}
