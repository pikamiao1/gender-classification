cmake_minimum_required(VERSION 2.8)
project( genderDet )

set(EXECUTABLE_OUTPUT_PATH "../")
set(LIBRARY_OUTPUT_PATH "../")

#add_subdirectory("./src")

find_package( OpenCV 2.4.9)
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package( Boost 1.60.0 REQUIRED COMPONENTS system filesystem)
include_directories( ${Boost_INCLUDE_DIRS} )
#link_directories( ${Boost_LIBRARY_DIRS})

add_library (myGabor myGabor.cpp)
add_library (pcaImpl pcaImpl.cpp)

add_executable( train trMain.cpp utils.cpp)
add_executable( test testMain.cpp utils.cpp)
#add_executable( LiveDemo CameraGenderRecognition.cpp utils.cpp)

target_link_libraries( train ${OpenCV_LIBS} ${Boost_LIBRARIES} myGabor pcaImpl)
target_link_libraries( test ${OpenCV_LIBS} ${Boost_LIBRARIES} myGabor pcaImpl)
#target_link_libraries( LiveDemo ${OpenCV_LIBS} ${Boost_LIBRARIES} myGabor pcaImpl)
