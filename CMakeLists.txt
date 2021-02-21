cmake_minimum_required(VERSION 3.15)
project(simulator)

set( SIM_LIB ${PROJECT_NAME} )
set(CMAKE_CXX_STANDARD 17)
find_package( OpenCV REQUIRED )

IF (UNIX)
    AUX_SOURCE_DIRECTORY(ocv_stages SOURCE_FILES)
ENDIF()

if(WIN32)
    link_directories("C:/OpenCV-4.5.1/opencv/build/x64/vc15/lib")
ENDIF()


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake RESULT_VARIABLE CONAN_PATH)

if (NOT CONAN_PATH)
    message(FATAL_ERROR "Did you forget to run conan install .. from ${CMAKE_BINARY_DIR}?")
ELSE()
    message(STATUS "Found conanbuildinfo.cmake at ${CONAN_PATH}")
ENDIF()
conan_basic_setup()

add_library(${SIM_LIB} STATIC core/src/registers/analogue_register.cpp core/include/simulator/registers/analogue_register.h core/src/registers/digital_register.cpp core/include/simulator/registers/digital_register.h core/src/buses/analogue_bus.cpp core/include/simulator/buses/analogue_bus.h core/src/buses/digital_bus.cpp core/include/simulator/buses/digital_bus.h core/src/base/photodiode.cpp core/include/simulator/base/photodiode.h core/src/base/array.cpp core/include/simulator/base/array.h core/src/base/processing_element.cpp core/include/simulator/base/processing_element.h core/src/units/comparator.cpp core/include/simulator/units/comparator.h core/src/units/squarer.cpp core/include/simulator/units/squarer.h core/src/registers/register.cpp core/include/simulator/registers/register.h core/src/base/component.cpp core/include/simulator/base/component.h core/src/memory/dram/dram.cpp core/include/simulator/memory/dram.h core/src/memory/sram/sram.cpp core/include/simulator/memory/sram.h core/src/memory/memory_type.cpp core/include/simulator/memory/memory_type.h core/src/memory/dram/dram_3t.cpp core/include/simulator/memory/dram_3t.h core/src/memory/si/si.cpp core/include/simulator/memory/si.h core/src/metrics/stats.cpp core/include/simulator/metrics/stats.h core/src/util/utility.cpp core/include/simulator/util/utility.h core/src/metrics/cycle_counter.cpp core/include/simulator/metrics/cycle_counter.h core/src/memory/sram/sram_6t.cpp core/include/simulator/memory/sram_6t.h core/src/metrics/json_writer.cpp core/include/simulator/metrics/json_writer.h)

if(WIN32)
    string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
    if (CMAKE_BUILD_TYPE STREQUAL "DEBUG")
        message(STATUS "Debug mode")
        target_link_libraries(${SIM_LIB} PRIVATE ${CONAN_LIBS} ${OpenCV_libs} opencv_world451d)
    ELSEIF(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
        message(STATUS "Release mode")
        target_link_libraries(${SIM_LIB} PRIVATE ${CONAN_LIBS} ${OpenCV_libs} opencv_world451)
    ENDIF()
ELSEIF(UNIX)
    target_link_libraries(${SIM_LIB} PRIVATE ${CONAN_LIBS} ${OpenCV_LIBS})
ENDIF()

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core/include>
        $<INSTALL_INTERFACE:core/include>
        )

target_include_directories(${SIM_LIB} PRIVATE ${OpenCV_INCLUDE_DIRS})


#######
#Architectures
#######
SET( ARCH scamp5 )
add_executable(${ARCH} architectures/main.cpp architectures/scamp5.h architectures/scamp5.cpp)
if(WIN32)
    string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
    if (CMAKE_BUILD_TYPE STREQUAL "DEBUG")
        target_link_libraries(${ARCH} PRIVATE ${CONAN_LIBS} ${OpenCV_libs} simulator opencv_world451d)
    ELSEIF(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
        target_link_libraries(${ARCH} PRIVATE ${CONAN_LIBS} ${OpenCV_libs} simulator opencv_world451)
    ENDIF()
ELSEIF(UNIX)
    target_link_libraries(${ARCH} PRIVATE ${CONAN_LIBS} ${OpenCV_LIBS} simulator)
ENDIF()
target_include_directories(${ARCH} PUBLIC ${OpenCV_INCLUDE_DIRS} core/include )

