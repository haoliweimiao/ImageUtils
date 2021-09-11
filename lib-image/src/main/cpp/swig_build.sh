#!/bin/bash

swig -c++ -java -package com.von.image.utils.lib -outdir ../java/com/von/image/utils/lib/  -o ./SwigImageUtils.cpp ./ImageUtils.i