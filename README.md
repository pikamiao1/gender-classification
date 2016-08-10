# face-master
Aug 2016

Project for COMP7502  
Gender Classification  

Group 23: LIN Zhen, XU Linxiao, ZHU Yilei  


# dependency

opencv 2.4.9 <br />
boost 1.60.0 <br />

**MacOS:** <br />
brew install opencv boost <br />

**ubuntu:** <br />
apt-get install opencv boost <br />


# build  

cd face-master <br />
cd src  <br />
rm CMakeCache.txt (if any) <br /> 

cmake . <br />
make <br />


# run

## training program  

**usage: ./train /path/to/test/folder [det]**  <br />

train folder must have 2 subdir: female and male  <br />

e.g. <br />
    ./train trainSet ----------- # will look for folder: trainSet_output, if exists，then will not do face-detection again <br />
    ./train trainSet det ------- # force re-do face-detection  <br />


## test program  

**usage: ./test /path/to/test/file/or/folder [cal]**  <br />

test folder for calulation must have 2 subdir: female and male <br />

e.g. <br />
    ./test testpic/1.jpg ------ # output gender result only  <br />
    ./test testSet cal -------- # output calssfication precision <br />


# Result and video demo

Result: <br />
both female and male recognition precesion up to **79%** <br />

Demo: <br />
https://www.youtube.com/watch?v=aWE7YZFow1o&feature=youtu.be <br />


# file list

## files under first level

README.md ------------------------# current file  <br />
haarcascade_frontalface_alt.xml - # face detection from opencv 2.4.9  <br />

src/ ---------------------------- # all source file  <br />
trainSet/ ----------------------- # training set, including female and male pics <br />
testPic/ ------------------------ # test picutres <br />
testSet/ ------------------------ # test set for precision evaluation <br />

ignore/ ------------------------- # reference file and scripts, can ignore <br />


## source files under src/

CMakeLists.txt ------------------ # cmake file  <br />

trMain.cpp ---------------------- # entry point for training <br />
testMain.cpp -------------------- # entry point for test <br />

myGabor.h ----------------------- # OUR own feature extraction algorithm <br />
myGabor.cpp <br />

pcaImpl.h ----------------------- # OUR own PCA algorithm  <br />
pcaImpl.cpp <br />

utils.h ------------------------- # shared by trMain and testMain <br />
utils.cpp <br />
