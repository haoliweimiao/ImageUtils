#include <jni.h>
#include <malloc.h>

#include "ImageUtils.hpp"

jint nv21ToI420(jBytePoint srcData, jBytePoint dstData, int width, int height) {
    jint src_y_size = width * height;
    jint src_u_size = (width >> 1) * (height >> 1);

    jBytePoint src_nv21_y_data = srcData;
    jBytePoint src_nv21_vu_data = srcData + src_y_size;

    jBytePoint src_i420_y_data = dstData;
    jBytePoint src_i420_u_data = dstData + src_y_size;
    jBytePoint src_i420_v_data = dstData + src_y_size + src_u_size;

    jint result = libyuv::NV21ToI420((const uint8_t *) src_nv21_y_data, width,
                                     (const uint8_t *) src_nv21_vu_data, width,
                                     (uint8_t *) src_i420_y_data, width,
                                     (uint8_t *) src_i420_u_data, width >> 1,
                                     (uint8_t *) src_i420_v_data, width >> 1,
                                     width, height);
    return result;
}

jint i420Rotate(jBytePoint srcData, jBytePoint dstData, int width, int height, int rotation) {
    jint src_i420_y_size = width * height;
    jint src_i420_u_size = (width >> 1) * (height >> 1);
    jBytePoint src_i420_y_data = srcData;
    jBytePoint src_i420_u_data = srcData + src_i420_y_size;
    jBytePoint src_i420_v_data = srcData + src_i420_y_size + src_i420_u_size;

    jBytePoint dst_i420_y_data = dstData;
    jBytePoint dst_i420_u_data = dstData + src_i420_y_size;
    jBytePoint dst_i420_v_data = dstData + src_i420_y_size + src_i420_u_size;

    int dst_stride_y, dst_stride_uv;
    if (rotation == 0 || rotation == 180) {
        dst_stride_y = width;
        dst_stride_uv = width >> 1;
    } else {
        dst_stride_y = height;
        dst_stride_uv = height >> 1;
    }

    jint result = libyuv::I420Rotate((const uint8_t *) src_i420_y_data, width,
                                     (const uint8_t *) src_i420_u_data, width >> 1,
                                     (const uint8_t *) src_i420_v_data, width >> 1,
                                     (uint8_t *) dst_i420_y_data, dst_stride_y,
                                     (uint8_t *) dst_i420_u_data, dst_stride_uv,
                                     (uint8_t *) dst_i420_v_data, dst_stride_uv,
                                     width, height,
                                     (libyuv::RotationMode) rotation);

    return result;
}

jint
i420Scale(jBytePoint srcData, int width, int height, jBytePoint dstData, int dst_width, int dst_height,
          int mode) {
    jint src_i420_y_size = width * height;
    jint src_i420_u_size = (width >> 1) * (height >> 1);
    jBytePoint src_i420_y_data = srcData;
    jBytePoint src_i420_u_data = srcData + src_i420_y_size;
    jBytePoint src_i420_v_data = srcData + src_i420_y_size + src_i420_u_size;

    jint dst_i420_y_size = dst_width * dst_height;
    jint dst_i420_u_size = (dst_width >> 1) * (dst_height >> 1);
    jBytePoint dst_i420_y_data = dstData;
    jBytePoint dst_i420_u_data = dstData + dst_i420_y_size;
    jBytePoint dst_i420_v_data = dstData + dst_i420_y_size + dst_i420_u_size;

    jint result = libyuv::I420Scale((const uint8_t *) src_i420_y_data, width,
                                    (const uint8_t *) src_i420_u_data, width >> 1,
                                    (const uint8_t *) src_i420_v_data, width >> 1,
                                    width, height,
                                    (uint8_t *) dst_i420_y_data, dst_width,
                                    (uint8_t *) dst_i420_u_data, dst_width >> 1,
                                    (uint8_t *) dst_i420_v_data, dst_width >> 1,
                                    dst_width, dst_height,
                                    (libyuv::FilterMode) mode);

    return result;
}

jint i420Mirror(jBytePoint srcData, jBytePoint dstData, int width, int height) {
    jint src_i420_y_size = width * height;
    jint src_i420_u_size = (width >> 1) * (height >> 1);

    jBytePoint src_i420_y_data = srcData;
    jBytePoint src_i420_u_data = srcData + src_i420_y_size;
    jBytePoint src_i420_v_data = srcData + src_i420_y_size + src_i420_u_size;

    jBytePoint dst_i420_y_data = dstData;
    jBytePoint dst_i420_u_data = dstData + src_i420_y_size;
    jBytePoint dst_i420_v_data = dstData + src_i420_y_size + src_i420_u_size;

    jint result = libyuv::I420Mirror((const uint8_t *) src_i420_y_data, width,
                                     (const uint8_t *) src_i420_u_data, width >> 1,
                                     (const uint8_t *) src_i420_v_data, width >> 1,
                                     (uint8_t *) dst_i420_y_data, width,
                                     (uint8_t *) dst_i420_u_data, width >> 1,
                                     (uint8_t *) dst_i420_v_data, width >> 1,
                                     width, height);

    return result;
}

jint i420Crop(jBytePoint src_i420_data, jBytePoint dst_i420_data, int width, int height, int dst_width,
              int dst_height, int src_length, int left, int top) {
    //裁剪的区域大小不对
    if (left + dst_width > width || top + dst_height > height) {
        return -1;
    }

    //left和top必须为偶数，否则显示会有问题
    if (left % 2 != 0 || top % 2 != 0) {
        return -1;
    }

    jint dst_i420_y_size = dst_width * dst_height;
    jint dst_i420_u_size = (dst_width >> 1) * (dst_height >> 1);

    jBytePoint dst_i420_y_data = dst_i420_data;
    jBytePoint dst_i420_u_data = dst_i420_data + dst_i420_y_size;
    jBytePoint dst_i420_v_data = dst_i420_data + dst_i420_y_size + dst_i420_u_size;

    jint result = libyuv::ConvertToI420((const uint8_t *) src_i420_data, src_length,
                                        (uint8_t *) dst_i420_y_data, dst_width,
                                        (uint8_t *) dst_i420_u_data, dst_width >> 1,
                                        (uint8_t *) dst_i420_v_data, dst_width >> 1,
                                        left, top,
                                        width, height,
                                        dst_width, dst_height,
                                        libyuv::kRotate0, libyuv::FOURCC_I420);
    return result;
}

jint i420ToNV21(jBytePoint src_i420_data, jBytePoint src_nv21_data, int width, int height) {
    jint src_y_size = width * height;
    jint src_u_size = (width >> 1) * (height >> 1);

    jBytePoint src_i420_y_data = src_i420_data;
    jBytePoint src_i420_u_data = src_i420_data + src_y_size;
    jBytePoint src_i420_v_data = src_i420_data + src_y_size + src_u_size;

    jBytePoint src_nv21_y_data = src_nv21_data;
    jBytePoint src_nv21_vu_data = src_nv21_data + src_y_size;


    jint result = libyuv::I420ToNV21(
            (const uint8_t *) src_i420_y_data, width,
            (const uint8_t *) src_i420_u_data, width >> 1,
            (const uint8_t *) src_i420_v_data, width >> 1,
            (uint8_t *) src_nv21_y_data, width,
            (uint8_t *) src_nv21_vu_data, width,
            width, height);

    return result;
}

jint i420Transform(jBytePoint srcI420Data, jBytePoint dstI420Data, int width, int height, int rotate,
                   jboolean mirror) {

    jint length = width * height * 3 / 2;
    int ret = 0;
    auto *i420MirrorCache = new jbyte[length];
    if (mirror) {
        // 镜像后的缓存先赋值给 dstData
        ret = i420Mirror(srcI420Data, i420MirrorCache, width, height);
        if (ret < 0) {
            delete[] i420MirrorCache;
            return ret;
        }
    }

    // 旋转后的缓存先赋值给dstData
    ret = i420Rotate(mirror ? i420MirrorCache : srcI420Data, dstI420Data, width, height,
                     rotate);

    delete[] i420MirrorCache;

    return ret;
}

jint nv21Transform(jBytePoint srcNv21Data, jBytePoint dstData, int width, int height, int rotate,
                   jboolean mirror) {
    jint length = width * height * 3 / 2;

    auto *i420Cache = new jbyte[length];

    // 将转换后的I420数据缓存在cacheData
    int ret = nv21ToI420(srcNv21Data, i420Cache, width, height);
    if (ret < 0) {
        delete[] i420Cache;
        return ret;
    }

    auto *i420TransformCache = new jbyte[length];
    ret = i420Transform(i420Cache, i420TransformCache, width, height, rotate, mirror);
    delete[] i420Cache;

    if (ret < 0) {
        delete[] i420TransformCache;
        return ret;
    }

    ret = i420ToNV21(i420TransformCache, dstData, width, height);
    delete[] i420TransformCache;
    return ret;
}

jint yuy2ToI420(jBytePoint src_yuy2_data, jBytePoint dst_i420_data, int width, int height) {
    jint src_i420_y_size = width * height;
    jint src_i420_u_size = (width >> 1) * (height >> 1);

    jBytePoint dst_i420_y_data = dst_i420_data;
    jBytePoint dst_i420_u_data = dst_i420_data + src_i420_y_size;
    jBytePoint dst_i420_v_data = dst_i420_data + src_i420_y_size + src_i420_u_size;

    jint result = libyuv::YUY2ToI420(
            (const uint8_t *) src_yuy2_data, width * 2,
            (uint8_t *) dst_i420_y_data, width,
            (uint8_t *) dst_i420_u_data, width >> 1,
            (uint8_t *) dst_i420_v_data, width >> 1,
            width, height);

    return result;
}

jint yuy2ToNV21(jBytePoint srcData, jBytePoint dstData,
                jint width, jint height, jint rotate, jboolean mirror){

    jint i420Length = width * height * 3 / 2;
    auto *i420Cache = new jbyte[i420Length];
    int ret = yuy2ToI420(srcData, i420Cache, width, height);
    if (ret < 0) {
        delete[] i420Cache;
        return ret;
    }

    auto *i420TransformCache = new jbyte[i420Length];
    ret = i420Transform(i420Cache, i420TransformCache, width, height, rotate, mirror);
    delete[] i420Cache;
    if (ret < 0) {
        delete[] i420TransformCache;
        return ret;
    }

    ret = i420ToNV21(i420TransformCache, dstData, width, height);
    delete[] i420TransformCache;

    return ret;
}