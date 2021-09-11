//
// Created by von on 2021/9/7.
//

#ifndef IMAGE_UTILS_HPP
#define IMAGE_UTILS_HPP

#include <string>
#include <cstring>
#include <ctime>
#include <libyuv.h>
#include <libyuv/rotate.h>

#include "log.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * typedef jbyte *jBytePoint;
 * To adapt swig, use the following types
 */
typedef signed char *jBytePoint;

jint i420Rotate(jBytePoint srcData, jBytePoint dstData, int width, int height, int rotation);

jint
i420Scale(jBytePoint srcData, int width, int height, jBytePoint dstData, int dst_width, int dst_height,
          int mode);

jint i420Mirror(jBytePoint srcData, jBytePoint dstData, int width, int height);

jint i420ToNV21(jBytePoint srcData, jBytePoint dstData, int width, int height);

jint i420Crop(jBytePoint srcData, jBytePoint dstData, int width, int height, int dst_width,
              int dst_height, int src_length, int left, int top);

jint i420Transform(jBytePoint srcData, jBytePoint dstData, int width, int height, int rotate,
                   jboolean mirror);

jint nv21ToI420(jBytePoint srcData, jBytePoint dstData, int width, int height);

jint nv21Transform(jBytePoint srcData, jBytePoint dstData, int width, int height, int rotate,
                   jboolean mirror);

jint yuy2ToI420(jBytePoint srcData, jBytePoint dstData, int width, int height);

jint yuy2ToNV21(jBytePoint srcData, jBytePoint dstData,
                jint width, jint height, jint rotate, jboolean mirror);

#ifdef __cplusplus
}
#endif

#endif //IMAGE_UTILS_HPP
