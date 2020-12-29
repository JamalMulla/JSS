cmake_minimum_required(VERSION 3.15)
project(Simulator)

find_package( OpenCV REQUIRED )
set(CMAKE_CXX_STANDARD 17)
AUX_SOURCE_DIRECTORY(ocv_stages SOURCE_FILES)

set(CMAKE_CXX_STANDARD 11)


add_executable(Simulator main.cpp PE/ProcessingElement.cpp PE/ProcessingElement.h PE/AREG.cpp PE/AREG.h PE/DREG.cpp PE/DREG.h PE/ASRC.cpp PE/ASRC.h PE/DSRC.cpp PE/DSRC.h Video.cpp Video.h Test.cpp Test.h)
target_link_libraries( Simulator ${OpenCV_LIBS} )