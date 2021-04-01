set(UNIT_TEST_LIST buses/analogue_bus)

foreach(NAME IN LISTS UNIT_TEST_LIST)
    list(APPEND UNIT_TEST_SOURCE_LIST ${NAME}_test.cpp)
endforeach()

set(TARGET_NAME tests)

add_executable(${TARGET_NAME} main.cpp ${UNIT_TEST_SOURCE_LIST} utility.cpp utility.h buses/digital_bus_test.cpp)

target_link_libraries(${TARGET_NAME}
        PUBLIC ${CONAN_LIBS} ${OpenCV_LIBS} simulator)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})