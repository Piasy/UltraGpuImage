//
// Created by Piasy on 2018/12/17.
//

#ifndef ULTRAGPUIMAGE_UGI_GL_H
#define ULTRAGPUIMAGE_UGI_GL_H

#if defined(__ANDROID__)

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h> // https://stackoverflow.com/q/31003863/3077508

#elif defined(__APPLE__)

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

#else
#error Unknown platform
#endif

#endif //ULTRAGPUIMAGE_UGI_GL_H
