//
// Created by Piasy on 2018/12/17.
//

#include "ugi_renderer.h"

#include "ugi_logging.h"

namespace Ugi {

static constexpr GLint kUgiTextureEnum = GL_TEXTURE0;
static constexpr GLuint kVertexIndices[] = {
        0, 1, 3, 1, 2, 3,
};

static const GLchar* kVertexShader
        = UGI_SHADER(
                layout(location = 0) in vec2 aVertCoords;
                layout(location = 1) in vec2 aTexCoords;
                out vec2 texCoords;
                void main() {
                    gl_Position = vec4(aVertCoords, 0.0, 1.0);
                    texCoords = aTexCoords;
                }
        );
static const GLchar* kFragmentShaderRgb
        = UGI_SHADER_PRECISION_M(
                  in vec2 texCoords;
                  uniform sampler2D tex;
                  out vec4 fragColor;
                  void main() {
                      fragColor = texture(tex, texCoords);
                  }
          );
static const GLchar* kFragmentShaderOes
        = "#extension GL_OES_EGL_image_external : require\n"
        UGI_SHADER_PRECISION_M(
                  in vec2 texCoords;
                  uniform sampler2D tex;
                  out vec4 fragColor;
                  void main() {
                      fragColor = texture(tex, texCoords);
                  }
          );

Renderer::Renderer(Transformation transformation) : vao_(0), vbo_(0), ebo_(0),
                                                    transformation_(transformation) {
    for (int i = 0; i < kTextureTypeMax; i++) {
        programs_[i] = 0;
    }
}

Renderer::~Renderer() {
}

void Renderer::OnSurfaceCreated() {
    glGenVertexArrays(1, &vao_);

    GLuint buffers[] = {0, 0};
    glGenBuffers(2, buffers);
    vbo_ = buffers[0];
    ebo_ = buffers[1];

    bindBuffers();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Renderer::OnSurfaceDestroyed() {
    for (int i = 0; i < kTextureTypeMax; i++) {
        if (programs_[i] != 0) {
            glDeleteProgram(programs_[i]);
        }
    }

    GLuint buffers[] = {vbo_, ebo_};
    glDeleteBuffers(2, buffers);
    glDeleteVertexArrays(1, &vao_);
}

void Renderer::UpdateTransformation(Transformation transformation) {
    if (transformation_ != transformation) {
        ugi_log(UGI_LOG_LEVEL_INFO, "UpdateTransformation from %s to %s",
                transformation_.Describe(), transformation.Describe());

        transformation_ = transformation;
        bindBuffers();
    } else {
        ugi_log(UGI_LOG_LEVEL_INFO, "UpdateTransformation with same %s", transformation.Describe());
    }
}

void Renderer::RenderRgb(GLuint texture_id, int32_t width, int32_t height, int64_t timestamp) {
    renderTexture(kTextureTypeRgb, texture_id, width, height, timestamp);
}

void Renderer::RenderOes(GLuint texture_id, int32_t width, int32_t height, int64_t timestamp) {
    renderTexture(kTextureTypeOes, texture_id, width, height, timestamp);
}

void Renderer::renderTexture(TextureType type, GLuint texture_id, int32_t width, int32_t height,
                             int64_t timestamp) {
    prepareShader(type);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(programs_[type]);

    glActiveTexture(kUgiTextureEnum);
    glBindTexture(textureType(type), texture_id);

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::prepareShader(Renderer::TextureType type) {
    if (programs_[type] != 0) {
        return;
    }

    const GLchar* fragmentShader = nullptr;
    switch (type) {
        case kTextureTypeOes:
            fragmentShader = kFragmentShaderOes;
            break;
        case kTextureTypeRgb:
        default:
            fragmentShader = kFragmentShaderRgb;
            break;
    }
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &kVertexShader, nullptr);
    glCompileShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragmentShader, nullptr);
    glCompileShader(fragment_shader);

    programs_[type] = glCreateProgram();
    glAttachShader(programs_[type], vertex_shader);
    glAttachShader(programs_[type], fragment_shader);
    glLinkProgram(programs_[type]);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    GLint tex_loc = glGetUniformLocation(programs_[type], "tex");

    glUseProgram(programs_[type]);
    glUniform1i(tex_loc, kUgiTextureEnum - GL_TEXTURE0);
}

GLenum Renderer::textureType(Renderer::TextureType type) {
    switch (type) {
        case kTextureTypeOes:
            return GL_TEXTURE_EXTERNAL_OES;
        case kTextureTypeRgb:
        default:
            return GL_TEXTURE_2D;
    }
}

void Renderer::bindBuffers() {
    glViewport(0, 0, transformation_.output_width(), transformation_.output_height());
    transformation_.Resolve(vertex_attributes_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_attributes_), vertex_attributes_, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kVertexIndices), kVertexIndices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                          reinterpret_cast<const void*>(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}
