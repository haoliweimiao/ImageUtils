#!/bin/bash

swig -c++ -java -package com.von.image.jpeg.lib -outdir ../java/com/von/image/jpeg/lib/  -o ./JpegTurboWrap.cpp ./JpegTurbo.i