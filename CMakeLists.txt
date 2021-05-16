cmake_minimum_required(VERSION 3.15)
project(simulator)

set(SIM_LIB ${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 20)
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

add_executable(${SIM_LIB} main.cpp src/simulator/registers/analogue_register.cpp include/simulator/registers/analogue_register.h src/simulator/registers/digital_register.cpp include/simulator/registers/digital_register.h src/simulator/buses/analogue_bus.cpp include/simulator/buses/analogue_bus.h src/simulator/buses/digital_bus.cpp include/simulator/buses/digital_bus.h src/simulator/base/pixel.cpp include/simulator/base/pixel.h src/simulator/base/architecture.cpp include/simulator/base/architecture.h src/simulator/pe/processing_element.cpp include/simulator/pe/processing_element.h src/simulator/units/comparator.cpp include/simulator/units/comparator.h src/simulator/units/squarer.cpp include/simulator/units/squarer.h src/simulator/registers/register.cpp include/simulator/registers/register.h src/simulator/base/component.cpp include/simulator/base/component.h src/simulator/memory/dram/dram3t_cell.cpp include/simulator/memory/dram3t_cell.h src/simulator/memory/sram/sram6t_cell.cpp include/simulator/memory/sram6t_cell.h src/simulator/memory/memory.cpp include/simulator/memory/memory.h src/simulator/memory/si/si_cell.cpp include/simulator/memory/si_cell.h src/simulator/util/utility.cpp include/simulator/util/utility.h src/simulator/metrics/cycle_counter.cpp include/simulator/metrics/cycle_counter.h src/simulator/metrics/json_writer.cpp include/simulator/metrics/json_writer.h include/simulator/ui/async_file_streamer.h include/simulator/ui/async_file_reader.h src/simulator/ui/ui.cpp include/simulator/ui/ui.h src/simulator/ui/file_watcher.cpp include/simulator/ui/file_watcher.h include/simulator/ui/base64_encoder.h src/simulator/ui/src/base64_encoder.cpp include/simulator/input/input_source.h src/simulator/input/live_input.cpp include/simulator/input/live_input.h src/simulator/input/image_input.cpp include/simulator/input/image_input.h src/simulator/input/video_input.cpp include/simulator/input/video_input.h include/simulator/base/plane_params.h src/simulator/base/plane_params.cpp include/simulator/adders/cla.h src/simulator/adders/cla.cpp include/simulator/base/config.h src/simulator/base/config.cpp include/simulator/metrics/stats_outputter.h src/simulator/memory/dram/dram_array.cpp include/simulator/memory/dram_array.h include/simulator/external/parser.h src/simulator/external/parser.cpp scamp5/scamp5.h scamp5/scamp5.cpp scamp5/analognet2/conv_instructions.h scamp5/analognet2/fc_weights.h scamp5/analognet2/analog_main.h scamp5/analognet2/analog_main.cpp scamp5_extended/scamp5_e.cpp scamp5_extended/scamp5_e.h scamp5_multiplexed/scamp5m.cpp scamp5_multiplexed/scamp5m.h include/simulator/alu/alu.h src/simulator/alu/alu.cpp include/simulator/adc/adc.h src/simulator/adc/adc.cpp include/simulator/metrics/packer.h src/simulator/metrics/packer.cpp include/simulator/metrics/pack_node.h src/simulator/metrics/pack_node.cpp scamp5_multiplexed/scamp5rmalt.cpp scamp5_multiplexed/scamp5rmalt.h)

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


target_include_directories(${SIM_LIB} PRIVATE ${OpenCV_INCLUDE_DIRS})

target_link_libraries(${SIM_LIB} PUBLIC ${CONAN_LIBS})
target_include_directories(${SIM_LIB}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )

#######
# Tests
#######
enable_testing()
include(CTest)
include(Catch)
catch_discover_tests(${SIM_LIB})
add_subdirectory(tests)
