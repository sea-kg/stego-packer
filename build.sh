#!/bin/bash
qmake packer.pro
make

./packer pack test.png test_.png "hello friends it's very simple packer text to image"
md5sum test.png
md5sum test_.png
./packer unpack test_.png
