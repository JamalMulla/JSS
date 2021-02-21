cmake_minimum_required(VERSION 3.15)
project(simulator)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake RESULT_VARIABLE CONAN_PATH)

if (NOT CONAN_PATH)
    message(FATAL_ERROR "Did you forget to run conan install .. from ${CMAKE_BINARY_DIR}?")
ELSE()
    message(STATUS "Found conanbuildinfo.cmake at ${CONAN_PATH}")
ENDIF()
conan_basic_setup()

add_library(${PROJECT_NAME} STATIC core/src/registers/analogue_register.cpp core/src/registers/analogue_register.h core/src/registers/digital_register.cpp core/src/registers/digital_register.h core/src/buses/analogue_bus.cpp core/src/buses/analogue_bus.h core/src/buses/digital_bus.cpp core/src/buses/digital_bus.h core/src/base/photodiode.cpp core/src/base/photodiode.h core/src/base/array.cpp core/src/base/array.h core/src/base/processing_element.cpp core/src/base/processing_element.h core/src/units/comparator.cpp core/src/units/comparator.h core/src/units/Squarer.cpp core/src/units/Squarer.h core/src/registers/register.cpp core/src/registers/register.h core/src/base/component.cpp core/src/base/component.h core/src/memory/dram/dram.cpp core/src/memory/dram/dram.h core/src/memory/sram/sram.cpp core/src/memory/sram/sram.h core/src/memory/memory_type.cpp core/src/memory/memory_type.h core/src/memory/dram/dram_3t.cpp core/src/memory/dram/dram_3t.h core/src/memory/si/si.cpp core/src/memory/si/si.h core/src/metrics/stats.cpp core/src/metrics/stats.h core/src/util/utility.cpp core/src/util/utility.h core/src/metrics/cycle_counter.cpp core/src/metrics/cycle_counter.h core/src/memory/sram/sram_6t.cpp core/src/memory/sram/sram_6t.h core/src/metrics/json_writer.cpp core/src/metrics/json_writer.h)

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

#######
#Architectures
#######
SET( ARCH scamp5 )
add_executable(${ARCH} architectures/main.cpp architectures/scamp5.h architectures/scamp5.cpp)
target_include_directories(${ARCH} PUBLIC core/include)
target_link_libraries(${ARCH} PUBLIC simulator)
