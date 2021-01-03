cmake_minimum_required(VERSION 3.15)
project(Simulator)

find_package( OpenCV REQUIRED )
set(CMAKE_CXX_STANDARD 17)

#AUX_SOURCE_DIRECTORY(ocv_stages SOURCE_FILES)

#if (CMAKE_BUILD_TYPE EQUAL "Release")
#    message("Release mode")
#    include(cmake-build-release/conanbuildinfo.cmake)
#else()
#    message("Debug mode")
#    include(cmake-build-debug/conanbuildinfo.cmake)
#endif ()

#conan_basic_setup()
link_directories("C:/OpenCV-4.5.1/opencv/build/x64/vc15/lib")

add_executable(${PROJECT_NAME} main.cpp PE/ProcessingElement.cpp PE/ProcessingElement.h PE/AREG.cpp PE/AREG.h PE/DREG.cpp PE/DREG.h PE/ASRC.cpp PE/ASRC.h PE/DSRC.cpp PE/DSRC.h Video.cpp Video.h Test.cpp Test.h)

target_include_directories( ${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries( ${PROJECT_NAME} PRIVATE ${OpenCV_libs} opencv_world451d)