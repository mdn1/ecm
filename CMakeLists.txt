cmake_minimum_required(VERSION 3.3.0)
project(embedded_cockpit_manager VERSION 0.1.0)

include(CTest)
enable_testing()


add_library(
    gpio_board
    gpio_board.cpp
    gpio_board.hpp
)

add_library(
    pcf8574_io    
    pcf8574_io.cpp
    pcf8574_io.hpp
)

add_library(
    tcp_message
    tcp_message.cpp
    tcp_message.hpp
)

add_library(
    tcp
    tcp.cpp
    tcp.hpp
)

find_package (Threads)

add_executable(embedded_cockpit_manager main.cpp)

target_link_libraries(embedded_cockpit_manager PRIVATE gpio_board pcf8574_io tcp_message tcp ${CMAKE_THREAD_LIBS_INIT})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
