//
// Created by Piasy on 2018/12/22.
//

#ifndef ULTRAGPUIMAGE_UGI_FILTER_GROUP_H
#define ULTRAGPUIMAGE_UGI_FILTER_GROUP_H

#include <vector>
#include <memory>

#include "ugi_filter.h"

namespace Ugi {

class FilterGroup : public Filter {
public:
    FilterGroup();

    void AddFilter(Filter* filter);

    int Init(int32_t output_width, int32_t output_height) override;

    int Apply(TextureType type, GLuint texture_id, GLuint vao) override;

    int Destroy() override;

    bool IsGroup() override;

private:
    void flattenFilters();

    void destroyFbo();

    std::vector<std::shared_ptr<Filter>> filters_;
    std::vector<std::shared_ptr<Filter>> flat_filters_;

    int32_t fbo_num_;
    GLuint* fbo_;
    GLuint* fbo_textures_;

    GLuint vao_fc_;
    GLuint vbo_fc_;
    GLuint ebo_fc_;
    GLfloat vertex_attributes_fc_[16];

    bool initialized_;
    bool error_;

    int32_t output_width_;
    int32_t output_height_;
};

}

#endif //ULTRAGPUIMAGE_UGI_FILTER_GROUP_H
