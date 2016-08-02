TARGET = main

OPENCV_ROOT = /usr/accel/cv
INCS = -I $(OPENCV_ROOT)/include
LIBS = -L $(OPENCV_ROOT)/lib -lopencv_core -lopencv_imgproc -lopencv_imgcodecs

BOOST_ROOT = /usr/accel/boost
INCS += -I $(BOOST_ROOT)/include
LIBS += -L $(BOOST_ROOT)/lib64 -lboost_system-mt -lboost_filesystem-mt

default: main.cpp
		g++ -O3 -fopenmp $(INCS) main.cpp $(LIBS) -o $(TARGET)

.PHONY: clean
clean:
		rm -f $(TARGET)

