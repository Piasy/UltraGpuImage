//
// Created by Piasy on 2018/12/17.
//

#include "ugi_renderer.h"

#include "ugi_logging.h"

namespace Ugi {

static constexpr GLuint kVertexIndices[] = {
        0, 1, 3, 1, 2, 3,
};

Renderer::Renderer(Transformation transformation) : vao_(0), vbo_(0), ebo_(0),
                                                    transformation_(transformation) {
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
    if (filter_) {
        filter_->Destroy();
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

void Renderer::SetFilter(Filter* filter) {
    filter_.reset(filter);
}

void Renderer::RenderTexture(TextureType type, GLuint texture_id) {
    if (!filter_ || filter_->Init() != 0) {
        return;
    }

    filter_->Apply(type, texture_id, vao_);
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
