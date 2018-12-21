//
// Created by Piasy on 2018/12/21.
//

#ifndef ULTRAGPUIMAGE_UGI_LOGGING_H
#define ULTRAGPUIMAGE_UGI_LOGGING_H

#if __cplusplus
extern "C" {
#endif

#define UGI_LOG_LEVEL_INFO 3
#define UGI_LOG_LEVEL_ERROR 4

typedef void (* ugi_log_func)(int level, const char* log);
void ugi_set_log_func(ugi_log_func log_func);

void ugi_log(int level, const char* fmt, ...);

#if __cplusplus
}
#endif

#endif //ULTRAGPUIMAGE_UGI_LOGGING_H
