set(FILES
   ${CMAKE_CURRENT_SOURCE_DIR}/tls_common.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/tls_common.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/http_requests.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/http_requests.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/mbedtls_config.h
)

add_library(tls INTERFACE)
target_include_directories(tls INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(tls INTERFACE 
    pico_cyw43_arch_lwip_poll
    pico_lwip_mbedtls
    pico_mbedtls
    pico_stdlib
)
target_sources(tls INTERFACE ${FILES})