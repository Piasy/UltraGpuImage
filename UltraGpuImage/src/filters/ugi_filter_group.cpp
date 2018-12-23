//
// Created by Piasy on 2018/12/22.
//

#include <ugi_gl_utils.h>
#include "ugi_filter_group.h"

#include "ugi_transformation.h"
#include "ugi_gl_utils.h"

namespace Ugi {

FilterGroup::FilterGroup() : Filter("", ""),
                                  fbo_num_(0), fbo_(nullptr), fbo_textures_(nullptr),
                                  initialized_(false), error_(false),
                                  output_width_(0), output_height_(0) {}

void FilterGroup::AddFilter(Filter* filter) {
    filters_.push_back(std::shared_ptr<Filter>(filter));
    flattenFilters();
}

int FilterGroup::Init(int32_t output_width, int32_t output_height) {
    if (initialized_ || error_) {
        return 0;
    }

    for (auto& filter : flat_filters_) {
        int result = filter->Init(output_width, output_height);
        if (result != 0) {
            error_ = true;
            return result;
        }
    }

    if (!flat_filters_.empty()
        && (output_width_ != output_width || output_height_ != output_height)) {
        if (fbo_) {
            destroyFbo();
        }

        output_width_ = output_width;
        output_height_ = output_height;

        fbo_num_ = static_cast<int32_t>(flat_filters_.size() - 1);
        fbo_ = new GLuint[fbo_num_];
        fbo_textures_ = new GLuint[fbo_num_];
        glGenFramebuffers(fbo_num_, fbo_);
        glGenTextures(fbo_num_, fbo_textures_);

        for (int32_t i = 0; i < fbo_num_; i++) {
            glBindTexture(GL_TEXTURE_2D, fbo_textures_[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, output_width_, output_height_, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, nullptr);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glBindFramebuffer(GL_FRAMEBUFFER, fbo_[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                                   fbo_textures_[i], 0);

            glBindTexture(GL_TEXTURE_2D, 0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }

    GLuint buffers[] = {0, 0, 0};
    glGenVertexArrays(1, buffers);
    glGenBuffers(2, buffers + 1);
    vao_fc_ = buffers[0];
    vbo_fc_ = buffers[1];
    ebo_fc_ = buffers[2];

    Transformation::Size size(output_width_, output_height_);
    Transformation transformation(size, size);
    transformation.UpdateScaleType(Transformation::kScaleTypeFitXY);
    transformation.Resolve(vertex_attributes_fc_);

    GlUtils::BindBuffers(vertex_attributes_fc_, vao_fc_, vbo_fc_, ebo_fc_);

    initialized_ = true;
    return 0;
}

int FilterGroup::Apply(TextureType type, GLuint texture_id, GLuint vao) {
    if (error_) {
        return -1;
    }
    if (!fbo_) {
        return -2;
    }

    GLuint prev_texture_id = texture_id;
    for (size_t i = 0; i < flat_filters_.size(); i++) {
        bool notLast = i < flat_filters_.size() - 1;
        auto& filter = flat_filters_[i];
        if (notLast) {
            glBindFramebuffer(GL_FRAMEBUFFER, fbo_[i]);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glViewport(0, 0, output_width_, output_height_);
        }

        if (i == 0) {
            filter->Apply(type, prev_texture_id, vao);
        } else {
            filter->Apply(kTextureTypeRgb, prev_texture_id, vao_fc_);
        }

        if (notLast) {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            prev_texture_id = fbo_textures_[i];
        }
    }

    return 0;
}

int FilterGroup::Destroy() {
    for (auto& filter : flat_filters_) {
        filter->Destroy();
    }

    destroyFbo();

    GLuint buffers[] = {vao_fc_, vbo_fc_, ebo_fc_};
    glDeleteVertexArrays(1, buffers);
    glDeleteBuffers(2, buffers + 1);

    return 0;
}

bool FilterGroup::IsGroup() {
    return true;
}

void FilterGroup::flattenFilters() {
    flat_filters_.clear();

    for (auto& filter : filters_) {
        if (filter->IsGroup()) {
            FilterGroup* group = reinterpret_cast<FilterGroup*>(filter.get());
            group->flattenFilters();
            flat_filters_.insert(flat_filters_.end(),
                                 group->flat_filters_.begin(), group->flat_filters_.end());
        } else {
            flat_filters_.push_back(filter);
        }
    }
}

void FilterGroup::destroyFbo() {
    if (fbo_textures_) {
        glDeleteTextures(fbo_num_, fbo_textures_);
        delete fbo_textures_;
        fbo_textures_ = nullptr;
    }
    if (fbo_) {
        glDeleteFramebuffers(fbo_num_, fbo_);
        delete[] fbo_;
        fbo_ = nullptr;
    }
    fbo_num_ = 0;
}

}
