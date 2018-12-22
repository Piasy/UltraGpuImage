//
// Created by Piasy on 2018/12/19.
//

#ifndef ULTRAGPUIMAGE_UGI_TRANSFORMATION_H
#define ULTRAGPUIMAGE_UGI_TRANSFORMATION_H


#include <stdint.h>

#include "ugi_gl.h"

namespace Ugi {

class Transformation {
public:
    enum Flip {
        kFlipNone = 0,
        kFlipHorizontal = 1,
        kFlipVertical = 2,
        kFlipHorizontalVertical = 3,
    };

    enum Rotation {
        kRotation_0 = 0,
        kRotation_90 = 90,
        kRotation_180 = 180,
        kRotation_270 = 270,
    };

    enum ScaleType {
        kScaleTypeFitXY = 0,
        kScaleTypeCenterCrop = 1,
        kScaleTypeCenterInside = 2,
    };

    static constexpr int32_t kCoordsScale = 10000;

    struct Rect {
        int32_t x;
        int32_t y;
        int32_t w;
        int32_t h;

        Rect(int32_t x, int32_t y, int32_t w, int32_t h);

        friend bool operator==(const Rect& lhs, const Rect& rhs);

        friend bool operator!=(const Rect& lhs, const Rect& rhs);
    };

    struct Size {
        int32_t w;
        int32_t h;

        Size(int32_t w, int32_t h);

        friend bool operator==(const Size& lhs, const Size& rhs);

        friend bool operator!=(const Size& lhs, const Size& rhs);
    };

    Transformation(Size input, Size output);

    void UpdateInput(Size input);

    void UpdateOutput(Size output);

    void UpdateCrop(Rect crop);

    void UpdateFlip(Flip flip);

    void UpdateRotation(Rotation rotation);

    void UpdateScaleType(ScaleType scale_type);

    void Resolve(GLfloat vertex_attributes[16]);

    const char* Describe();

    int32_t input_width();

    int32_t input_height();

    int32_t output_width();

    int32_t output_height();

    Rotation rotation();

    Flip flip();

    friend bool operator==(const Transformation& lhs, const Transformation& rhs);

    friend bool operator!=(const Transformation& lhs, const Transformation& rhs);

private:
    void reset();

    void resolveCrop();

    void resolveFlip();

    void resolveRotate();

    void resolveScale();

    void swap(GLfloat coords[16], int index1, int index2);

    const char* flipName();

    const char* scale_type();

    Rect crop_;
    Size input_;
    Size output_;
    Rotation rotation_;
    Flip flip_;
    ScaleType scale_type_;

    GLfloat vertex_coords_[8];
    GLfloat texture_coords_[8];
};

}

#endif //ULTRAGPUIMAGE_UGI_TRANSFORMATION_H
