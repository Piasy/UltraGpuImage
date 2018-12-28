//
// Created by Piasy on 2018/12/21.
//

#include <stdio.h>
#include <stdarg.h>

#include "ugi_logging.h"

extern "C" {

static ugi_log_func g_log_func = nullptr;

void ugi_set_log_func(ugi_log_func log_func) {
    g_log_func = log_func;
}

void ugi_log(int level, const char* fmt, ...) {
    if (g_log_func) {
        char buf[1024];

        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);

        g_log_func(level, buf);
    }
}

}
