SET(ARCH scamp5)
add_executable(${ARCH} main.cpp scamp5.h scamp5.cpp analognet2/conv_instructions.h analognet2/fc_weights.h analognet2/analog_main.h analognet2/analog_main.cpp)
#if (WIN32)
#    string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
#    if (CMAKE_BUILD_TYPE STREQUAL "DEBUG")
#        target_link_libraries(${ARCH} PRIVATE ${CONAN_LIBS} ${OpenCV_libs} simulator opencv_world451d)
#    ELSEIF (CMAKE_BUILD_TYPE STREQUAL "RELEASE")
#        target_link_libraries(${ARCH} PRIVATE ${CONAN_LIBS} ${OpenCV_libs} simulator opencv_world451)
#    ENDIF ()
#ELSEIF (UNIX)
#    target_link_libraries(${ARCH} PRIVATE ${CONAN_LIBS} ${OpenCV_LIBS} simulator)
#ENDIF ()
#include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake RESULT_VARIABLE CONAN_PATH)
#
#if (NOT CONAN_PATH)
#    message(FATAL_ERROR "Did you forget to run conan install .. from ${CMAKE_BINARY_DIR}?")
#ELSE ()
#    message(STATUS "Found conanbuildinfo.cmake at ${CONAN_PATH}")
#ENDIF ()
#set(CONAN_SYSTEM_INCLUDES ON)
#conan_basic_setup()
target_link_libraries(${ARCH} PUBLIC ${CONAN_LIBS} simulator)
#target_include_directories(${ARCH} PUBLIC include)
target_include_directories(${ARCH}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        include
        )
