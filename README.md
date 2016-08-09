# face-master

project for COMP7502
Group 23, Aug 2016

LIN Zhen    3035238333
XU Linxiao  3035237896
ZHU Yilei   3035268948


//============
// dependency
//============

opencv 2.4.9
boost 1.60.0

MacOS:
    brew install opencv boost

ubuntu:
    apt-get install opencv boost


//=======
// build
//=======

cd face-master
cd src
rm CMakeCache.txt (if any)

cmake .
make


//======
// run
//======

// training program
//-------------------------------

usage: ./train /path/to/test/folder [det]
# train folder must have 2 subdir: female and male

# e.g.
    ./train trainSet       # will look for folder: trainSet_output
                           # if exists，then will not do face-detection again
    ./train trainSet det   # force re-do face-detection


// test program
//-------------------------------

usage: ./test /path/to/test/file/or/folder [cal]

# e.g.
    ./test testpic/1.jpg        # output gender result only
    ./test testSet       cal    # output calssfication precision
                                # test folder for calulation must have 2 subdir: female and male


//===========
// file list
//===========

README.md                         # current file
haarcascade_frontalface_alt.xml   # face detection from opencv 2.4.9

src/                              # all source file
trainSet/                         # training set, including female and male pics
testPic/                          # test picutres
testSet/                          # test set for precision evaluation

ignore/                           # reference file and scripts, can ignore


// source files under src/
//-------------------------

CMakeLists.txt                    # cmake file

trMain.cpp                        # entry point for training
testMain.cpp                      # entry point for test

myGabor.h                         # OUR own feature extraction algorithm
myGabor.cpp

pcaImpl.h                         # OUR own PCA algorithm
pcaImpl.cpp

utils.h                            # shared by trMain and testMain
utils.cpp


//========
// result
//========

Demo:
    https://www.youtube.com/watch?v=aWE7YZFow1o&feature=youtu.be

Result:
    precesion up to 79%

