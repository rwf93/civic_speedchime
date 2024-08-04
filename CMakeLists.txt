cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

set(PROJECT honda_civic_speedchime)
project(${PROJECT})

pico_sdk_init()

add_executable(${PROJECT} main.c)

pico_set_program_name(${PROJECT} "${PROJECT}")
pico_set_program_version(${PROJECT} "0.1")

pico_enable_stdio_usb(${PROJECT} 1)
pico_enable_stdio_uart(${PROJECT} 1)

target_link_libraries(${PROJECT} pico_stdlib pico_multicore hardware_pwm hardware_pio hardware_adc pico_time pico_multicore)

pico_generate_pio_header(${PROJECT} ${CMAKE_CURRENT_LIST_DIR}/pio_reader.pio)

pico_add_extra_outputs(${PROJECT})
