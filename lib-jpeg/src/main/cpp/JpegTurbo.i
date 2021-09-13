%module NativeJpegTurbo

/* Anything in the following section is added verbatim to the .cxx wrapper file */
%inline %{
    #include "jpeglib.h"
    #include "turbojpeg.h"
    #include "JpegTurbo.h"
%}

#ifdef SWIGJAVA
%include "enumtypeunsafe.swg"
%javaconst(1);

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("turbojpeg");
            System.loadLibrary("jpeg");
//            System.loadLibrary("test");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Failed to load native library jpeg\n" + e);
        }
    }
%}
#endif

/* This will allow us to iterate through arrays defined by STL containers */
%include "std_string.i"
%include "std_vector.i"

/* typedef jBytePoint same resource code */
typedef signed char *ImageData;
typedef unsigned long *ImageSize;

%include <typemaps.i>
/* c code fun(jBytePoint srcData,...) -> java code fun(byte[],...) */
%apply ImageData INOUT { ImageData i420Buffer };
%apply ImageData INOUT { ImageData jpegBuffer };
%apply ImageSize INOUT { ImageSize jpeg_size };

/* This is the list of headers to be wrapped */
/* For Java, it seems we need the file of interest and all files up the inheritance tree */
//%include "JpegTurbo.h"

int yuv2jpeg(ImageData i420Buffer, int yuvSize, int yuvWidth, int yuvHeight,
             ImageData jpegBuffer, ImageSize jpeg_size, int quality);