cmake_minimum_required(VERSION 3.15)
project(Simulator)

set(CMAKE_CXX_STANDARD 17)

add_executable(Simulator main.cpp PE/ProcessingElement.cpp PE/ProcessingElement.h PE/AREG.cpp PE/AREG.h PE/DREG.cpp PE/DREG.h PE/ASRC.cpp PE/ASRC.h PE/DSRC.cpp PE/DSRC.h)