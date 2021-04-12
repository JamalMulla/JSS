cmake_minimum_required(VERSION 3.15)
project(simulator)

set(SIM_LIB ${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)

string(
        APPEND _opts
        "$<IF:$<CXX_COMPILER_ID:MSVC>,"
        "/W4;$<$<CONFIG:RELEASE>:/O2>,"
        "-Wall;-Wextra;"
        "$<$<CONFIG:RELEASE>:-O3>"
        "$<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>"
        ">"
)

add_compile_options("${_opts}")

option(USE_RUNTIME_CHECKS "Check arguments and emit warnings" OFF)
option(TRACK_STATISTICS "Tracks statistics such as register reads and energy use" ON)
if (USE_RUNTIME_CHECKS)
    message(STATUS "With runtime checks")
    add_definitions(-DUSE_RUNTIME_CHECKS)
endif ()
unset(USE_RUNTIME_CHECKS CACHE)
if (TRACK_STATISTICS)
    message(STATUS "Tracking statistics")
    add_definitions(-DTRACK_STATISTICS)
endif ()
unset(TRACK_STATISTICS CACHE)


IF (UNIX)
    AUX_SOURCE_DIRECTORY(ocv_stages SOURCE_FILES)
ENDIF ()

if (WIN32)
    link_directories("C:/OpenCV-4.5.1/opencv/build/x64/vc15/lib")
ENDIF ()


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake RESULT_VARIABLE CONAN_PATH)

if (NOT CONAN_PATH)
    message(FATAL_ERROR "Did you forget to run conan install .. from ${CMAKE_BINARY_DIR}?")
ELSE ()
    message(STATUS "Found conanbuildinfo.cmake at ${CONAN_PATH}")
ENDIF ()
set(CONAN_SYSTEM_INCLUDES ON)
conan_basic_setup()

add_library(${SIM_LIB} STATIC src/simulator/registers/analogue_register.cpp include/simulator/registers/analogue_register.h src/simulator/registers/digital_register.cpp include/simulator/registers/digital_register.h src/simulator/buses/analogue_bus.cpp include/simulator/buses/analogue_bus.h src/simulator/buses/digital_bus.cpp include/simulator/buses/digital_bus.h src/simulator/base/pixel.cpp include/simulator/base/pixel.h src/simulator/base/array.cpp include/simulator/base/array.h src/simulator/base/processing_element.cpp include/simulator/base/processing_element.h src/simulator/units/comparator.cpp include/simulator/units/comparator.h src/simulator/units/squarer.cpp include/simulator/units/squarer.h src/simulator/registers/register.cpp include/simulator/registers/register.h src/simulator/base/component.cpp include/simulator/base/component.h src/simulator/memory/dram/dram3t_cell.cpp include/simulator/memory/dram3t_cell.h src/simulator/memory/sram/sram6t_cell.cpp include/simulator/memory/sram6t_cell.h src/simulator/memory/memory.cpp include/simulator/memory/memory.h src/simulator/memory/si/si.cpp include/simulator/memory/si.h src/simulator/metrics/stats.cpp include/simulator/metrics/stats.h src/simulator/util/utility.cpp include/simulator/util/utility.h src/simulator/metrics/cycle_counter.cpp include/simulator/metrics/cycle_counter.h src/simulator/metrics/json_writer.cpp include/simulator/metrics/json_writer.h include/simulator/ui/async_file_streamer.h include/simulator/ui/async_file_reader.h src/simulator/ui/ui.cpp include/simulator/ui/ui.h src/simulator/ui/file_watcher.cpp include/simulator/ui/file_watcher.h include/simulator/ui/base64_encoder.h src/simulator/ui/src/base64_encoder.cpp include/simulator/input/input_source.h src/simulator/input/live_input.cpp include/simulator/input/live_input.h src/simulator/input/image_input.cpp include/simulator/input/image_input.h src/simulator/input/video_input.cpp include/simulator/input/video_input.h include/simulator/base/plane_params.h src/simulator/base/plane_params.cpp include/simulator/adders/cla.h src/adder/cla.cpp include/simulator/base/config.h)

if (WIN32)
    string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
    if (CMAKE_BUILD_TYPE STREQUAL "DEBUG")
        message(STATUS "Debug mode")
        target_link_libraries(${SIM_LIB} PRIVATE ${CONAN_LIBS} ${OpenCV_libs} opencv_world451d)
    ELSEIF (CMAKE_BUILD_TYPE STREQUAL "RELEASE")
        message(STATUS "Release mode")
        target_link_libraries(${SIM_LIB} PRIVATE ${CONAN_LIBS} ${OpenCV_libs} opencv_world451)
    ENDIF ()
ELSEIF (UNIX)
    target_link_libraries(${SIM_LIB} PRIVATE ${CONAN_LIBS} ${OpenCV_LIBS})
ENDIF ()

target_include_directories(${SIM_LIB}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )

target_include_directories(${SIM_LIB} PRIVATE ${OpenCV_INCLUDE_DIRS})


#Warnings
#if ( CMAKE_COMPILER_IS_GNUCC )
#    target_compile_options(${SIM_LIB} PRIVATE "-Wall -Wextra")
#endif()
#if ( MSVC )
#    target_compile_options(${SIM_LIB} PRIVATE "/W4")
#endif()


#######
# Tests
#######
enable_testing()
add_subdirectory(scamp5)
add_subdirectory(tests)
