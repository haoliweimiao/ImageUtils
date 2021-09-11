#ifndef __LOG_H
#define __LOG_H

#include <android/log.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "LibImageUtils", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "LibImageUtils", __VA_ARGS__))

#ifdef __cplusplus
}
#endif

#endif /* __LOG_H */