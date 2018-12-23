//
// Created by Piasy on 2018/12/22.
//

#include "ugi_color_invert_filter.h"

namespace Ugi {

static const GLchar* kFragmentShader
        = "#version " UGI_GLSL_VERSION "\n"
          "#ifdef GL_ES\n"
          "precision mediump float;\n"
          "#endif\n"
          "in vec2 texCoords;\n"
          "uniform sampler2D tex;\n"
          "out vec4 fragColor;\n"
          "void main() {\n"
          "    vec4 color = texture(tex, texCoords);\n"
          "    fragColor = vec4((1.0 - color.rgb), color.a);\n"
          "}\n";

ColorInvertFilter::ColorInvertFilter() : Filter(kDirectVertexShader, kFragmentShader) {}

}
