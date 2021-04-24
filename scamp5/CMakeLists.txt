SET(ARCH scamp5)
add_executable(${ARCH} main.cpp scamp5.h scamp5.cpp analognet2/conv_instructions.h analognet2/fc_weights.h analognet2/analog_main.h analognet2/analog_main.cpp)

target_link_libraries(${ARCH} PUBLIC ${CONAN_LIBS} simulator)
target_include_directories(${ARCH}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        include
        )
