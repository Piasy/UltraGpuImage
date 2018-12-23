//
// Created by Piasy on 2018/12/22.
//

#ifndef ULTRAGPUIMAGE_UGI_GL_UTILS_H
#define ULTRAGPUIMAGE_UGI_GL_UTILS_H

#include "ugi_gl.h"

namespace Ugi {

class GlUtils {

public:

    static void BindBuffers(GLfloat* vertex_attributes, GLuint vao, GLuint vbo, GLuint ebo);

};

}

#endif //ULTRAGPUIMAGE_UGI_GL_UTILS_H
