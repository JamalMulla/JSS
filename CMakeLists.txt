cmake_minimum_required(VERSION 3.5)
project(Simulator)

find_package( OpenCV REQUIRED )
set(CMAKE_CXX_STANDARD 17)

IF (UNIX)
    AUX_SOURCE_DIRECTORY(ocv_stages SOURCE_FILES)
ENDIF()

#AUX_SOURCE_DIRECTORY(ocv_stages SOURCE_FILES)

#if (CMAKE_BUILD_TYPE EQUAL "Release")
#    message("Release mode")
#    include(cmake-build-release/conanbuildinfo.cmake)
#else()
#    message("Debug mode")
#    include(cmake-build-debug/conanbuildinfo.cmake)
#endif ()

#conan_basic_setup()
if(WIN32)
    link_directories("C:/OpenCV-4.5.1/opencv/build/x64/vc15/lib")
ENDIF()


add_executable(${PROJECT_NAME} main.cpp PE/PE.cpp PE/PE.h Video.cpp Video.h src/registers/analogue_register.cpp src/registers/analogue_register.h src/registers/digital_register.cpp src/registers/digital_register.h src/buses/analogue_bus.cpp src/buses/analogue_bus.h src/buses/digital_bus.cpp src/buses/digital_bus.h src/base/photodiode.cpp src/base/photodiode.h src/base/array.cpp src/base/array.h src/base/processing_element.cpp src/base/processing_element.h src/units/comparator.cpp src/units/comparator.h src/units/Squarer.cpp src/units/Squarer.h architectures/scamp5.cpp architectures/scamp5.h src/registers/register.cpp src/registers/register.h)

target_include_directories( ${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
if(WIN32)
    message("windows")
    target_link_libraries( ${PROJECT_NAME} PRIVATE ${OpenCV_libs} opencv_world451d)
ELSEIF(UNIX)
    target_link_libraries( ${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
ENDIF()
