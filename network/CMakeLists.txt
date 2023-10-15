
set(FILES
   ${CMAKE_CURRENT_SOURCE_DIR}/ntp.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/ntp.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/timeutils.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/timeutils.hpp
)

add_library(network INTERFACE)
target_include_directories(network INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(network INTERFACE pico_stdlib pico_cyw43_arch_lwip_poll hardware_rtc)
target_sources(network INTERFACE ${FILES})   