//
// Created by Piasy on 2018/12/17.
//

#include "ugi_renderer.h"

static const GLchar* kVertexShader
        = UGI_SHADER(
                layout(location = 0) in vec2 aPos;
                void main() {
                    gl_Position = vec4(aPos, 0.0, 1.0);
                }
        );

static const GLchar* kFragmentShader
        = UGI_SHADER_PRECISION_M(
                  out vec4 fragColor;
                  void main() {
                      fragColor = vec4(1.0, 0.0, 0.0, 1.0);
                  }
          );

UgiRenderer::UgiRenderer() : program_(0), vao_(0), vbo_(0), ebo_(0) {
}

UgiRenderer::~UgiRenderer() {
}

void UgiRenderer::OnSurfaceChanged(int32_t width, int32_t height) {
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &kVertexShader, nullptr);
    glCompileShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &kFragmentShader, nullptr);
    glCompileShader(fragment_shader);

    program_ = glCreateProgram();
    glAttachShader(program_, vertex_shader);
    glAttachShader(program_, fragment_shader);
    glLinkProgram(program_);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    GLuint buffers[] = {0, 0};
    glGenBuffers(2, buffers);
    vbo_ = buffers[0];
    ebo_ = buffers[1];

    GLfloat vertices[] = {
            1.0f, 1.0f,     // right top
            1.0f, -1.0f,    // right bottom
            -1.0f, -1.0f,   // left bottom
            -1.0f, 1.0f,    // left top
    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint indices[] = {
            0, 1, 3, 1, 2, 3,
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glViewport(0, 0, width, height);
}

void UgiRenderer::OnSurfaceDestroyed() {
    glDeleteProgram(program_);

    GLuint buffers[] = {vbo_, ebo_};
    glDeleteBuffers(2, buffers);
    glDeleteVertexArrays(1, &vao_);
}

void UgiRenderer::RenderFrame(GLuint texture_id, int32_t width, int32_t height, int64_t timestamp) {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program_);
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
