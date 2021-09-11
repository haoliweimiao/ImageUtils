%module NativeImageUtils

/* Anything in the following section is added verbatim to the .cxx wrapper file */
%{
#include "ImageUtils.hpp"

%}

// Constants from PJSIP libraries

#ifdef SWIGJAVA
%include "enumtypeunsafe.swg"
%javaconst(1);

%pragma(java) jniclasscode=%{
  static {
    try {
	    System.loadLibrary("ImageUtils");
    } catch (UnsatisfiedLinkError e) {
	System.err.println("Failed to load native library ImageUtils\n" + e);
    }
  }
%}

#endif

/* This will allow us to iterate through arrays defined by STL containers */
%include "std_string.i"
%include "std_vector.i"

/* This is the list of headers to be wrapped */
/* For Java, it seems we need the file of interest and all files up the inheritance tree */
%include "ImageUtils.hpp"
