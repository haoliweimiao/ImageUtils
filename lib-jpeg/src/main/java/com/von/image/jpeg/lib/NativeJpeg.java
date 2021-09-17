package com.von.image.jpeg.lib;
public class ZkJpegNative {
    static {
        System.loadLibrary("JpegUtils");
    }

    /**
     * I420 to jpeg
     *
     * @param i420Buffer I420 image data
     * @param yuvSize    I420 image length
     * @param yuvWidth   I420 image width
     * @param yuvHeight  I420 image height
     * @param jpegBuffer output jpeg data
     * @param quality    output jpeg image quality
     * @return 0 : success
     */
    public static native int i420ToJpeg(byte[] i420Buffer, int yuvSize, int yuvWidth, int yuvHeight,
                                        byte[] jpegBuffer, int quality);
}
