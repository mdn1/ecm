add_executable(embedded_cockpit_manager main.cpp)

#Force usage of C++17
target_compile_features(embedded_cockpit_manager PRIVATE cxx_std_17)

find_package (Threads REQUIRED)

target_link_libraries(embedded_cockpit_manager PRIVATE 
    gpio_board 
    pcf8574_io 
    tcp_message 
    tcp 
    ${CMAKE_THREAD_LIBS_INIT})
