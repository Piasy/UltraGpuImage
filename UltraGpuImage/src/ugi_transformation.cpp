//
// Created by Piasy on 2018/12/19.
//

#include "ugi_transformation.h"

#include <sstream>

namespace Ugi {

static constexpr int kRightTopX = 0;
static constexpr int kRightTopY = 1;
static constexpr int kRightBottomX = 2;
static constexpr int kRightBottomY = 3;
static constexpr int kLeftBottomX = 4;
static constexpr int kLeftBottomY = 5;
static constexpr int kLeftTopX = 6;
static constexpr int kLeftTopY = 7;

Transformation::Rect::Rect(int32_t x, int32_t y, int32_t w, int32_t h) : x(x), y(y), w(w), h(h) {
}

bool operator==(const Transformation::Rect& lhs, const Transformation::Rect& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.w == rhs.w && lhs.h == rhs.h;
}

bool operator!=(const Transformation::Rect& lhs, const Transformation::Rect& rhs) {
    return !(lhs == rhs);
}

Transformation::Size::Size(int32_t w, int32_t h) : w(w), h(h) {
}

bool operator==(const Transformation::Size& lhs, const Transformation::Size& rhs) {
    return lhs.w == rhs.w && lhs.h == rhs.h;
}

bool operator!=(const Transformation::Size& lhs, const Transformation::Size& rhs) {
    return !(lhs == rhs);
}

Transformation::Transformation(Transformation::Size input, Transformation::Size output)
        : crop_(0, 0, kCoordsScale, kCoordsScale), input_(input), output_(output),
          rotation_(kRotation_0), flip_(kFlipNone), scale_type_(kScaleTypeCenterCrop) {
    reset();
}

void Transformation::Resolve(GLfloat vertex_attributes[16]) {
    reset();

    resolveCrop();
    resolveFlip();
    resolveRotate();
    resolveScale();

    vertex_attributes[0] = vertex_coords_[0];
    vertex_attributes[1] = vertex_coords_[1];
    vertex_attributes[2] = texture_coords_[0];
    vertex_attributes[3] = texture_coords_[1];
    vertex_attributes[4] = vertex_coords_[2];
    vertex_attributes[5] = vertex_coords_[3];
    vertex_attributes[6] = texture_coords_[2];
    vertex_attributes[7] = texture_coords_[3];
    vertex_attributes[8] = vertex_coords_[4];
    vertex_attributes[9] = vertex_coords_[5];
    vertex_attributes[10] = texture_coords_[4];
    vertex_attributes[11] = texture_coords_[5];
    vertex_attributes[12] = vertex_coords_[6];
    vertex_attributes[13] = vertex_coords_[7];
    vertex_attributes[14] = texture_coords_[6];
    vertex_attributes[15] = texture_coords_[7];
}

int32_t Transformation::output_width() {
    return output_.w;
}

int32_t Transformation::output_height() {
    return output_.h;
}

void Transformation::UpdateInput(Transformation::Size input) {
    input_ = input;
}

void Transformation::UpdateOutput(Transformation::Size output) {
    output_ = output;
}

void Transformation::UpdateCrop(Transformation::Rect crop) {
    crop_ = crop;
}

void Transformation::UpdateFlip(Transformation::Flip flip) {
    flip_ = flip;
}

void Transformation::UpdateRotation(Transformation::Rotation rotation) {
    rotation_ = rotation;
}

void Transformation::UpdateScaleType(Transformation::ScaleType scale_type) {
    scale_type_ = scale_type;
}

void Transformation::reset() {
    // right top
    vertex_coords_[0] = 1.0f;
    vertex_coords_[1] = 1.0f;
    texture_coords_[0] = 1.0f;
    texture_coords_[1] = 1.0f;

    // right bottom
    vertex_coords_[2] = 1.0f;
    vertex_coords_[3] = -1.0f;
    texture_coords_[2] = 1.0f;
    texture_coords_[3] = 0.0f;

    // left bottom
    vertex_coords_[4] = -1.0f;
    vertex_coords_[5] = -1.0f;
    texture_coords_[4] = 0.0f;
    texture_coords_[5] = 0.0f;

    // left top
    vertex_coords_[6] = -1.0f;
    vertex_coords_[7] = 1.0f;
    texture_coords_[6] = 0.0f;
    texture_coords_[7] = 1.0f;
}

void Transformation::resolveCrop() {
    GLfloat minX = crop_.x;
    GLfloat minY = crop_.y;
    GLfloat maxX = minX + crop_.w;
    GLfloat maxY = minY + crop_.h;

    // right top
    texture_coords_[kRightTopX] = maxX / (GLfloat) kCoordsScale;
    texture_coords_[kRightTopY] = maxY / (GLfloat) kCoordsScale;
    // right bottom
    texture_coords_[kRightBottomX] = maxX / (GLfloat) kCoordsScale;
    texture_coords_[kRightBottomY] = minY / (GLfloat) kCoordsScale;
    // left bottom
    texture_coords_[kLeftBottomX] = minX / (GLfloat) kCoordsScale;
    texture_coords_[kLeftBottomY] = minY / (GLfloat) kCoordsScale;
    // left top
    texture_coords_[kLeftTopX] = minX / (GLfloat) kCoordsScale;
    texture_coords_[kLeftTopY] = maxY / (GLfloat) kCoordsScale;
}

void Transformation::resolveFlip() {
    switch (flip_) {
        case kFlipHorizontal:
            swap(texture_coords_, kLeftBottomX, kRightBottomX);
            swap(texture_coords_, kLeftTopX, kRightTopX);
            break;
        case kFlipVertical:
            swap(texture_coords_, kLeftBottomY, kLeftTopY);
            swap(texture_coords_, kRightBottomY, kRightTopY);
            break;
        case kFlipHorizontalVertical:
            swap(texture_coords_, kLeftBottomX, kRightBottomX);
            swap(texture_coords_, kLeftTopX, kRightTopX);

            swap(texture_coords_, kLeftBottomY, kLeftTopY);
            swap(texture_coords_, kRightBottomY, kRightTopY);
            break;
        case kFlipNone:
        default:
            break;
    }
}

void Transformation::resolveRotate() {
    float x, y;
    switch (rotation_) {
        case kRotation_90:
            x = texture_coords_[kLeftBottomX];
            y = texture_coords_[kLeftBottomY];
            texture_coords_[kLeftBottomX] = texture_coords_[kLeftTopX];
            texture_coords_[kLeftBottomY] = texture_coords_[kLeftTopY];
            texture_coords_[kLeftTopX] = texture_coords_[kRightTopX];
            texture_coords_[kLeftTopY] = texture_coords_[kRightTopY];
            texture_coords_[kRightTopX] = texture_coords_[kRightBottomX];
            texture_coords_[kRightTopY] = texture_coords_[kRightBottomY];
            texture_coords_[kRightBottomX] = x;
            texture_coords_[kRightBottomY] = y;
            break;
        case kRotation_180:
            swap(texture_coords_, kLeftBottomX, kRightTopX);
            swap(texture_coords_, kLeftBottomY, kRightTopY);
            swap(texture_coords_, kRightBottomX, kLeftTopX);
            swap(texture_coords_, kRightBottomY, kLeftTopY);
            break;
        case kRotation_270:
            x = texture_coords_[kLeftBottomX];
            y = texture_coords_[kLeftBottomY];
            texture_coords_[kLeftBottomX] = texture_coords_[kRightBottomX];
            texture_coords_[kLeftBottomY] = texture_coords_[kRightBottomY];
            texture_coords_[kRightBottomX] = texture_coords_[kRightTopX];
            texture_coords_[kRightBottomY] = texture_coords_[kRightTopY];
            texture_coords_[kRightTopX] = texture_coords_[kLeftTopX];
            texture_coords_[kRightTopY] = texture_coords_[kLeftTopY];
            texture_coords_[kLeftTopX] = x;
            texture_coords_[kLeftTopY] = y;
            break;
        case kRotation_0:
        default:
            break;
    }
}

void Transformation::resolveScale() {
    if (scale_type_ == kScaleTypeFitXY) {
        // The default is FIT_XY
        return;
    }

    // Note: scale type need to be implemented by adjusting
    // the vertex_coords_ (not texture_coords_).
    if (input_.w * output_.h == input_.h * output_.w) {
        // Optional optimization: If input w/h aspect is the same as output's,
        // there is no need to adjust vertex_coords_ at all.
        return;
    }

    float inputAspect = input_.w / (float) input_.h;
    float outputAspect = output_.w / (float) output_.h;

    switch (scale_type_) {
        case kScaleTypeCenterCrop: {
            if (inputAspect < outputAspect) {
                float heightRatio = outputAspect / inputAspect;
                vertex_coords_[kLeftBottomY] *= heightRatio;
                vertex_coords_[kRightBottomY] *= heightRatio;
                vertex_coords_[kLeftTopY] *= heightRatio;
                vertex_coords_[kRightTopY] *= heightRatio;
            } else {
                float widthRatio = inputAspect / outputAspect;
                vertex_coords_[kLeftBottomX] *= widthRatio;
                vertex_coords_[kRightBottomX] *= widthRatio;
                vertex_coords_[kLeftTopX] *= widthRatio;
                vertex_coords_[kRightTopX] *= widthRatio;
            }
            break;
        }
        case kScaleTypeCenterInside: {
            if (inputAspect < outputAspect) {
                float widthRatio = inputAspect / outputAspect;
                vertex_coords_[kLeftBottomX] *= widthRatio;
                vertex_coords_[kRightBottomX] *= widthRatio;
                vertex_coords_[kLeftTopX] *= widthRatio;
                vertex_coords_[kRightTopX] *= widthRatio;
            } else {
                float heightRatio = outputAspect / inputAspect;
                vertex_coords_[kLeftBottomY] *= heightRatio;
                vertex_coords_[kRightBottomY] *= heightRatio;
                vertex_coords_[kLeftTopY] *= heightRatio;
                vertex_coords_[kRightTopY] *= heightRatio;
            }
            break;
        }
        default:
            break;
    }
}

void Transformation::swap(GLfloat coords[16], int index1, int index2) {
    GLfloat tmp = coords[index1];
    coords[index1] = coords[index2];
    coords[index2] = tmp;
}

const char* Transformation::Describe() {
    std::ostringstream desc;
    desc << "Transformation{"
         << "crop=(" << crop_.x << "," << crop_.y << "," << crop_.w << "," << crop_.h << "), "
         << "input=" << input_.w << "x" << input_.h << ", "
         << "output=" << output_.w << "x" << output_.h << ", "
         << "rotation=" << rotation_ << ", "
         << "flip=" << flip() << ", "
         << "scale_type=" << scale_type()
         << "}";
    return desc.str().c_str();
}

const char* Transformation::flip() {
    switch (flip_) {
        case kFlipHorizontal:
            return "Horizontal";
        case kFlipVertical:
            return "Vertical";
        case kFlipHorizontalVertical:
            return "HorizontalVertical";
        case kFlipNone:
        default:
            return "None";
    }
}

const char* Transformation::scale_type() {
    switch (scale_type_) {
        case kScaleTypeCenterCrop:
            return "CenterCrop";
        case kScaleTypeCenterInside:
            return "CenterInside";
        case kScaleTypeFitXY:
        default:
            return "FitXY";
    }
}

bool operator==(const Transformation& lhs, const Transformation& rhs) {
    return lhs.crop_ == rhs.crop_ && lhs.input_ == rhs.input_ && lhs.output_ == rhs.output_
           && lhs.rotation_ == rhs.rotation_ && lhs.flip_ == rhs.flip_
           && lhs.scale_type_ == rhs.scale_type_;
}

bool operator!=(const Transformation& lhs, const Transformation& rhs) {
    return !(lhs == rhs);
}

}
