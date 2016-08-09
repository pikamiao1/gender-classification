#!/bin/bash -eu

OPENCV_ROOT=/usr/accel/cv
export PATH=${OPENCV_ROOT}/bin:${PATH:-}
export LD_LIBRARY_PATH=${OPENCV_ROOT}/lib:${LD_LIBRARY_PATH:-}
export PYTHONPATH=${OPENCV_ROOT}/lib/python2.7/site-packages:${PYTHONPATH:-}

BOOST_ROOT=/usr/accel/boost
export LD_LIBRARY_PATH=${BOOST_ROOT}/lib:${LD_LIBRARY_PATH:-}

make clean
make

export OMP_NUM_THREADS=8
echo "Using ${OMP_NUM_THREADS} threads"

time ./main commands.cfg
