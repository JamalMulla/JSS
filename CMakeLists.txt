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

add_library(${PROJECT_NAME} STATIC core/src/registers/analogue_register.cpp core/include/simulator/registers/analogue_register.h core/src/registers/digital_register.cpp core/include/simulator/registers/digital_register.h core/src/buses/analogue_bus.cpp core/include/simulator/buses/analogue_bus.h core/src/buses/digital_bus.cpp core/include/simulator/buses/digital_bus.h core/src/base/photodiode.cpp core/include/simulator/base/photodiode.h core/src/base/array.cpp core/include/simulator/base/array.h core/src/base/processing_element.cpp core/include/simulator/base/processing_element.h core/src/units/comparator.cpp core/include/simulator/units/comparator.h core/src/units/squarer.cpp core/include/simulator/units/squarer.h core/src/registers/register.cpp core/include/simulator/registers/register.h core/src/base/component.cpp core/include/simulator/base/component.h core/src/memory/dram/dram.cpp core/include/simulator/memory/dram.h core/src/memory/sram/sram.cpp core/include/simulator/memory/sram.h core/src/memory/memory_type.cpp core/include/simulator/memory/memory_type.h core/src/memory/dram/dram_3t.cpp core/include/simulator/memory/dram_3t.h core/src/memory/si/si.cpp core/include/simulator/memory/si.h core/src/metrics/stats.cpp core/include/simulator/metrics/stats.h core/src/util/utility.cpp core/include/simulator/util/utility.h core/src/metrics/cycle_counter.cpp core/include/simulator/metrics/cycle_counter.h core/src/memory/sram/sram_6t.cpp core/include/simulator/memory/sram_6t.h core/src/metrics/json_writer.cpp core/include/simulator/metrics/json_writer.h)

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
target_include_directories(${PROJECT_NAME}
                           PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core/include>
                           $<INSTALL_INTERFACE:core/include>
                           )

#######
#Architectures
#######
SET( ARCH scamp5 )
add_executable(${ARCH} architectures/main.cpp architectures/scamp5.h architectures/scamp5.cpp)
target_include_directories(${ARCH} PUBLIC core/include )
target_link_libraries(${ARCH} PUBLIC simulator)
