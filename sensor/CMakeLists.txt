
set(FILES
   ${CMAKE_CURRENT_SOURCE_DIR}/datapoint.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/dht22.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/measurement.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/sensor.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/datapoint.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/dht22.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/measurement.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/sensor.hpp
)

add_library(sensor INTERFACE)
target_include_directories(sensor INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(sensor INTERFACE pico_stdlib hardware_rtc)
target_sources(sensor INTERFACE ${FILES})