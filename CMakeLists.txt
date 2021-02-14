cmake_minimum_required(VERSION 3.15)
project(Simulator)

find_package( OpenCV REQUIRED )
set(CMAKE_CXX_STANDARD 17)

IF (UNIX)
    AUX_SOURCE_DIRECTORY(ocv_stages SOURCE_FILES)
ENDIF()

if(WIN32)
    link_directories("C:/OpenCV-4.5.1/opencv/build/x64/vc15/lib")
ENDIF()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


add_executable(${PROJECT_NAME} main.cpp src/registers/analogue_register.cpp src/registers/analogue_register.h src/registers/digital_register.cpp src/registers/digital_register.h src/buses/analogue_bus.cpp src/buses/analogue_bus.h src/buses/digital_bus.cpp src/buses/digital_bus.h src/base/photodiode.cpp src/base/photodiode.h src/base/array.cpp src/base/array.h src/base/processing_element.cpp src/base/processing_element.h src/units/comparator.cpp src/units/comparator.h src/units/Squarer.cpp src/units/Squarer.h architectures/scamp5.cpp architectures/scamp5.h src/registers/register.cpp src/registers/register.h src/base/component.cpp src/base/component.h src/memory/dram/dram.cpp src/memory/dram/dram.h src/memory/sram/sram.cpp src/memory/sram/sram.h src/memory/memory_type.cpp src/memory/memory_type.h src/memory/dram/dram_3t.cpp src/memory/dram/dram_3t.h src/memory/si/si.cpp src/memory/si/si.h src/metrics/stats.cpp src/metrics/stats.h src/util/utility.cpp src/util/utility.h src/metrics/cycle_counter.cpp src/metrics/cycle_counter.h src/memory/sram/sram_6t.cpp src/memory/sram/sram_6t.h src/metrics/json_writer.cpp src/metrics/json_writer.h)

target_include_directories( ${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
if(WIN32)
    message("windows")
    target_link_libraries( ${PROJECT_NAME} PRIVATE ${OpenCV_libs} opencv_world451d)
ELSEIF(UNIX)
    target_link_libraries( ${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
ENDIF()

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
