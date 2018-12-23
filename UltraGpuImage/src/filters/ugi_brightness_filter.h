//
// Created by Piasy on 2018/12/23.
//

#ifndef ULTRAGPUIMAGE_UGI_BRIGHTNESS_FILTER_H
#define ULTRAGPUIMAGE_UGI_BRIGHTNESS_FILTER_H

#include "ugi_filter.h"

namespace Ugi {

class BrightnessFilter : public Filter {
public:
    BrightnessFilter();

    int Init(int32_t width, int32_t height) override;

    void PreDraw() override;

    void SetBrightness(GLfloat brightness);

private:
    GLint brightness_loc_;
    GLfloat brightness_;
};

}

#endif //ULTRAGPUIMAGE_UGI_BRIGHTNESS_FILTER_H
