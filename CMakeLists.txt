cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)
project(pico-logger C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_BUILD_TYPE Release)
pico_sdk_init()

add_executable(main
    main.cpp
)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 1)
pico_add_extra_outputs(main)

include_directories( "sensor" ) 
add_subdirectory( sensor )

include_directories( "network" ) 
add_subdirectory( network )

include_directories( "tls" ) 
add_subdirectory( tls )

include_directories( "influx" ) 
add_subdirectory( influx )


target_include_directories(main PRIVATE ${CMAKE_CURRENT_LIST_DIR} )

target_link_libraries(main 
    pico_stdlib
    pico_cyw43_arch_lwip_poll
    hardware_rtc
    pico_runtime 
    hardware_sleep
    sensor
    network
    tls
    influx
)