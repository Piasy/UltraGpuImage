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

#define _UGI_GET_MACRO_STRING_HELPER(x) #x
#define UGI_GET_MACRO_STRING(x) _UGI_GET_MACRO_STRING_HELPER(x)

#define _UGI_SHADER_VERSION_ 300 es

#ifdef _UGI_SHADER_VERSION_

#define UGI_GLES_ATTACH_L "#version " UGI_GET_MACRO_STRING(_UGI_SHADER_VERSION_) "\n#ifdef GL_ES\nprecision lowp float;\n#endif\n"
#define UGI_GLES_ATTACH_M "#version " UGI_GET_MACRO_STRING(_UGI_SHADER_VERSION_) "\n#ifdef GL_ES\nprecision mediump float;\n#endif\n"
#define UGI_GLES_ATTACH_H "#version " UGI_GET_MACRO_STRING(_UGI_SHADER_VERSION_) "\n#ifdef GL_ES\nprecision highp float;\n#endif\n"
#define UGI_GLES_ATTACH "#version " UGI_GET_MACRO_STRING(_UGI_SHADER_VERSION_) "\n"

#else

#define UGI_GLES_ATTACH_L "#ifdef GL_ES\nprecision lowp float;\n#endif\n"
#define UGI_GLES_ATTACH_M "#ifdef GL_ES\nprecision mediump float;\n#endif\n"
#define UGI_GLES_ATTACH_H "#ifdef GL_ES\nprecision highp float;\n#endif\n"
#define UGI_GLES_ATTACH
#endif

#define UGI_SHADER_PRECISION_L(string) UGI_GLES_ATTACH_L #string
#define UGI_SHADER_PRECISION_M(string) UGI_GLES_ATTACH_M #string
#define UGI_SHADER_PRECISION_H(string) UGI_GLES_ATTACH_H #string
#define UGI_SHADER(string) UGI_GLES_ATTACH #string

#endif //ULTRAGPUIMAGE_UGI_GL_H
