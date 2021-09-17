//
// Created by Von on 2021/7/23.
//

#ifndef EDK_LIBRARY_YUV_ZKJPEGTURBO_H
#define EDK_LIBRARY_YUV_ZKJPEGTURBO_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 *
 * @param i420Buffer  yuv数据
 * @param yuvSize    yuv大小,等于width*height*3/2
 * @param yuvWidth   图像宽度
 * @param yuvHeight  图像高度
 * @param subSample  enum TJSAMP:TJSAMP_444=0,TJSAMP_422,TJSAMP_420,TJSAMP_GRAY,TJSAMP_440,TJSAMP_411
 * @param jpegBuffer jpeg数据
 * @param jpegSize   jpeg数据大小
 * @param quality    转换后的图像质量(1~100)
 * @return
 */
int yuv2jpeg(unsigned char *i420Buffer, int yuvSize, int yuvWidth, int yuvHeight, int subSample,
             unsigned char **jpegBuffer, unsigned long *jpegSize, int quality);

#ifdef __cplusplus
}
#endif
#endif //EDK_LIBRARY_YUV_ZKJPEGTURBO_H
