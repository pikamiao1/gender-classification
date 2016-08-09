#!/bin/bash -eu

# opencv
VERSION=3.1.0



#==============
# dependencies
#==============

PYTHON_ROOT=$(dirname $(dirname $(which python)))

IPP_ROOT=$PWD/lib/ippicv_lnx

if [ ! -d ${IPP_ROOT} ]; then
    cd lib
    ./build.sh
    cd -
fi



#=========
# prepare
#=========

rm -fr opencv
rm -fr opencv.src
rm -fr opencv.build

unzip ${VERSION}.zip
mv    opencv-${VERSION}           opencv.src
cp -r opencv-${VERSION}-patches/. opencv.src/.



#=======
# build
#=======

mkdir -p opencv.build
cd       opencv.build

# libgphoto2 is excluded due to old version
cmake -D CMAKE_BUILD_TYPE=RELEASE \
      -D PYTHON_LIBRARY=${PYTHON_ROOT}/lib \
      -D PYTHON_INCLUDE_DIR=${PYTHON_ROOT}/include/python2.7 \
      -D IPPROOT=${IPP_ROOT} \
      -D WITH_CUBLAS=YES \
      -D WITH_GPHOTO2=NO \
      -D CMAKE_INSTALL_PREFIX=$PWD/../opencv \
      ../opencv.src

make -j8

make install

cd -

