set(TARGET_NAME tests)

add_executable(${TARGET_NAME} main.cpp utility.cpp utility.h buses/analogue_bus_test.cpp buses/digital_bus_test.cpp scamp5/scamp5_test.cpp ../scamp5/scamp5.cpp ../scamp5/scamp5.h)

target_link_libraries(${TARGET_NAME}
        PUBLIC ${CONAN_LIBS} ${OpenCV_LIBS} simulator)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})