#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include <malloc.h>
#include <android/log.h>
#include <bitset>
#include <iosfwd>

#include "jpeglib.h"
#include "turbojpeg.h"
#include "ZkJpegTurbo.h"

// JNI jbyteArray转char*
unsigned char *convertJByteaArrayToChars(JNIEnv *env, jbyteArray bytearray) {
    //  char *pmsg = ConvertJByteaArrayToChars(env,jmsgversion);
    //  ...
    //  delete pmsg;
    unsigned char *chars = NULL;
    jbyte *bytes;
    bytes = env->GetByteArrayElements(bytearray, 0);
    int chars_len = env->GetArrayLength(bytearray);
    chars = new unsigned char[chars_len + 1];
    memset(chars, 0, chars_len + 1);
    memcpy(chars, bytes, chars_len);
    chars[chars_len] = 0;

    env->ReleaseByteArrayElements(bytearray, bytes, 0);

    return chars;
}

int yuv2jpeg(unsigned char *yuv_buffer, int yuv_size, int width, int height, int subsample,
             unsigned char **jpeg_buffer, unsigned long *jpeg_size, int quality) {
    tjhandle handle = NULL;
    int flags = 0;
    int padding = 1; // 1或4均可，但不能是0
    int need_size = 0;
    int ret = 0;

    handle = tjInitCompress();

    flags |= 0;

    need_size = tjBufSizeYUV2(width, padding, height, subsample);
    if (need_size != yuv_size) {
        printf("we detect yuv size: %d, but you give: %d, check again.\n", need_size, yuv_size);
        return 0;
    }

    ret = tjCompressFromYUV(handle, yuv_buffer, width, padding, height, subsample, jpeg_buffer,
                            jpeg_size, quality, flags);
    if (ret < 0) {
        printf("compress to jpeg failed: %s\n", tjGetErrorStr());
    }

    tjDestroy(handle);

    return ret;
}

static jint i420ToJpeg(JNIEnv *env, jclass clazz, jbyteArray i420_buffer, jint yuv_size,
                       jint yuv_width, jint yuv_height, jbyteArray jpeg_buffer,
                       jint quality) {
    unsigned char *yuv420Data = convertJByteaArrayToChars(env, i420_buffer);

    int yuvSubsample = TJSAMP_420;
    unsigned long yuv2JpegSize;

    jboolean isCopy = false;
    jbyte *bBuffer = env->GetByteArrayElements(jpeg_buffer, &isCopy);
    auto *buf = (unsigned char *) bBuffer;
    yuv2jpeg(yuv420Data, yuv_size, yuv_width, yuv_height, yuvSubsample, &buf, &yuv2JpegSize,
             quality);
    delete yuv420Data;
    env->ReleaseByteArrayElements(jpeg_buffer, bBuffer, 0);

    return 0;
}

static jbyteArray i420ToJpegReturn(JNIEnv *env, jclass clazz, jbyteArray i420_buffer, jint yuv_size,
                                   jint yuv_width, jint yuv_height, jint quality) {
    unsigned char *yuv420Data = convertJByteaArrayToChars(env, i420_buffer);

    int yuvSubsample = TJSAMP_420;
    unsigned long jpeg_size;
    uint8_t *yuv2jpegBuffer;
    yuv2jpeg(yuv420Data, yuv_size, yuv_width, yuv_height, yuvSubsample, &yuv2jpegBuffer,
             &jpeg_size,
             quality);

    delete yuv420Data;

    // need delete returnData
    jbyteArray returnData = env->NewByteArray((jsize) jpeg_size);
    env->SetByteArrayRegion(returnData, 0, jpeg_size,
                            reinterpret_cast<const jbyte *>(yuv2jpegBuffer));
    return returnData;
}


static JNINativeMethod method_table[] = {
        {"i420ToJpeg", "([BIII[BI)I", (void *) i420ToJpeg},
};


extern "C" jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    } else {
        jclass clazz = env->FindClass("com/zkteco/edk/jpeg/ZkJpegNative");
        if (clazz) {
            jint ret = env->RegisterNatives(clazz, method_table,
                                            sizeof(method_table) / sizeof(method_table[0]));
            env->DeleteLocalRef(clazz);
            return ret == 0 ? JNI_VERSION_1_6 : JNI_ERR;
        } else {
            return JNI_ERR;
        }
    }
}
