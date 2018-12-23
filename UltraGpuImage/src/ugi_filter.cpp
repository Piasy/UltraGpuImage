//
// Created by Piasy on 2018/12/22.
//

#include <cstring>

#include "ugi_filter.h"
#include "ugi_logging.h"

namespace Ugi {

static constexpr GLint kUgiTextureEnum = GL_TEXTURE0;

const GLchar* Filter::kDirectVertexShader
        = "#version " UGI_GLSL_VERSION "\n"
          "layout(location = 0) in vec2 aVertCoords;\n"
          "layout(location = 1) in vec2 aTexCoords;\n"
          "out vec2 texCoords;\n"
          "void main() {\n"
          "    gl_Position = vec4(aVertCoords, 0.0, 1.0);\n"
          "    texCoords = aTexCoords;\n"
          "}\n";
const GLchar* Filter::kDirectFragmentShader
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

Filter::Filter() : Filter(kDirectVertexShader, kDirectFragmentShader) {
}

Filter::Filter(const GLchar* vertex_shader, const GLchar* fragment_shader)
        : vertex_shader_(vertex_shader),
          fragment_shader_(fragment_shader),
          program_(0) {
}

Filter::~Filter() {
}

int Filter::Init(int32_t output_width, int32_t output_height) {
    if (program_ != 0 || strlen(vertex_shader_) == 0 || strlen(fragment_shader_) == 0) {
        return 0;
    }

    program_ = CreateProgram(vertex_shader_, fragment_shader_);

    return 0;
}

int Filter::Apply(TextureType type, GLuint texture_id, GLuint vao) {
    return DoApply(program_, type, texture_id, vao);
}

int Filter::Destroy() {
    if (program_ == 0) {
        return 0;
    }

    glDeleteProgram(program_);
    program_ = 0;

    return 0;
}

void Filter::PreDraw() {
}

int Filter::DoApply(GLuint program, TextureType type, GLuint texture_id, GLuint vao) {
    if (program_ == 0) {
        return -1;
    }

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    glActiveTexture(kUgiTextureEnum);
    glBindTexture(textureType(type), texture_id);

    glBindVertexArray(vao);

    PreDraw();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    return 0;
}

GLenum Filter::textureType(TextureType type) {
    switch (type) {
        case kTextureTypeOes:
            return GL_TEXTURE_EXTERNAL_OES;
        case kTextureTypeRgb:
        default:
            return GL_TEXTURE_2D;
    }
}

GLuint Filter::CreateProgram(const GLchar* vertex_shader_src, const GLchar* fragment_shader_src) {
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_src, nullptr);
    glCompileShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_src, nullptr);
    glCompileShader(fragment_shader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    GLint tex_loc = glGetUniformLocation(program, "tex");

    glUseProgram(program);
    glUniform1i(tex_loc, kUgiTextureEnum - GL_TEXTURE0);

    return program;
}

bool Filter::IsGroup() {
    return false;
}

}
