cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(pico-logger C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(main
    main.cpp
)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 1)
pico_add_extra_outputs(main)

include_directories( "sensor" ) 
add_subdirectory( sensor )

target_include_directories(main PRIVATE ${CMAKE_CURRENT_LIST_DIR} )

target_link_libraries(main 
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    sensor
)