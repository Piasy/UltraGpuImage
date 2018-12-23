//
// Created by Piasy on 2018/12/22.
//

#ifndef ULTRAGPUIMAGE_UGI_PREPROCESS_FILTER_H
#define ULTRAGPUIMAGE_UGI_PREPROCESS_FILTER_H

#include "ugi_filter.h"

namespace Ugi {

class PreprocessFilter : public Filter {
public:
    int Init(int32_t output_width, int32_t output_height) override;

    int Apply(TextureType type, GLuint texture_id, GLuint vao) override;

    int Destroy() override;

private:
    GLuint oes_program_;
};

}

#endif //ULTRAGPUIMAGE_UGI_PREPROCESS_FILTER_H
