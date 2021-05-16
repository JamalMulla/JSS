set(TARGET_NAME tests)

add_executable(${TARGET_NAME} main.cpp utility.cpp utility.h buses/analogue_bus_test.cpp buses/digital_bus_test.cpp scamp5/scamp5_test.cpp ../scamp5/scamp5.cpp ../scamp5/scamp5.h ../scamp5_extended/scamp5_e.h ../scamp5_extended/scamp5_e.cpp scamp5e/scamp5e_test.cpp ../src/simulator/registers/register.cpp ../src/simulator/registers/analogue_register.cpp ../src/simulator/registers/digital_register.cpp ../src/simulator/buses/analogue_bus.cpp ../src/simulator/buses/digital_bus.cpp ../src/simulator/base/architecture.cpp ../src/simulator/base/pixel.cpp ../src/simulator/pe/processing_element.cpp ../src/simulator/memory/dram/dram_array.cpp ../src/simulator/util/utility.cpp ../src/simulator/base/config.cpp ../src/simulator/metrics/cycle_counter.cpp ../src/simulator/memory/memory.cpp ../src/simulator/input/live_input.cpp ../src/simulator/input/image_input.cpp ../src/simulator/input/video_input.cpp ../src/simulator/base/plane_params.cpp ../src/simulator/memory/sram/sram6t_cell.cpp ../src/simulator/memory/dram/dram3t_cell.cpp ../src/simulator/memory/si/si_cell.cpp ../src/simulator/external/parser.cpp ../src/simulator/ui/ui.cpp ../src/simulator/ui/file_watcher.cpp ../src/simulator/ui/src/base64_encoder.cpp memory/dram_array_test.cpp ../scamp5/news_t.h ../src/simulator/base/component.cpp ../src/simulator/metrics/packer.cpp ../src/simulator/metrics/pack_node.cpp)

target_link_libraries(${TARGET_NAME}
        PUBLIC ${CONAN_LIBS} ${OpenCV_LIBS})

target_include_directories(
        ${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
