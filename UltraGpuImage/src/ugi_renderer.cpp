//
// Created by Piasy on 2018/12/17.
//

#include "ugi_renderer.h"

#include "ugi_logging.h"
#include "ugi_gl_utils.h"

namespace Ugi {

Renderer::Renderer(Transformation transformation) : vao_(0), vbo_(0), ebo_(0),
                                                    transformation_(transformation),
                                                    filter_(nullptr) {
}

Renderer::~Renderer() {
}

void Renderer::OnSurfaceCreated() {
    GLuint buffers[] = {0, 0, 0};
    glGenVertexArrays(1, buffers);
    glGenBuffers(2, buffers + 1);
    vao_ = buffers[0];
    vbo_ = buffers[1];
    ebo_ = buffers[2];

    bindBuffers();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Renderer::OnSurfaceDestroyed() {
    if (filter_) {
        filter_->Destroy();
    }

    GLuint buffers[] = {vao_, vbo_, ebo_};
    glDeleteVertexArrays(1, buffers);
    glDeleteBuffers(2, buffers + 1);
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
    if (!filter_
        || filter_->Init(transformation_.output_width(), transformation_.output_height()) != 0) {
        return;
    }

    filter_->Apply(type, texture_id, vao_);
}

void Renderer::bindBuffers() {
    glViewport(0, 0, transformation_.output_width(), transformation_.output_height());
    transformation_.Resolve(vertex_attributes_);

    GlUtils::BindBuffers(vertex_attributes_, vao_, vbo_, ebo_);
}

}
