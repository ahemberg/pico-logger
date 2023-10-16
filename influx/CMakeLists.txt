set(FILES
   ${CMAKE_CURRENT_SOURCE_DIR}/influxclient.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/influxclient.hpp
)

add_library(influx INTERFACE)
target_include_directories(influx INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(influx INTERFACE 
    pico_stdlib
)
target_sources(tls INTERFACE ${FILES})