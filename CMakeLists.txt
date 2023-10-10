cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(pico-logger C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
#set(PICO_BOARD pico_w)
#set(PICO_SSD "askakreolstek")
#set(WIFI_PASSWORD "5qQYoCed4cF8FhRr")

#-DPICO_BOARD=pico_w -DPICO_SSID="askakreolstek" -DWIFI_PASSWORD="5qQYoCed4cF8FhRr" -DCMAKE_BUILD_TYPE=Debug

pico_sdk_init()

add_executable(main
    main.cpp
)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 1)
pico_add_extra_outputs(main)

target_include_directories(main PRIVATE ${CMAKE_CURRENT_LIST_DIR} )

target_link_libraries(main 
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
)