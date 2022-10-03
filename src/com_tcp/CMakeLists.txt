#Add all includes
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${embedded_cockpit_manager_SOURCE_DIR}/include/*.hpp")

#Path relative to this file where the files are located. E.g. gpio_board.cpp -> ./gpio_board.cpp
add_library( tcp_message tcp_message.cpp ${HEADER_LIST} )
add_library( tcp tcp.cpp ${HEADER_LIST} )

#If any of the source files from the libraries above are in a subdirectory relative to this files directory, 
#simply add it to the line below. E.g. "${CMAKE_CURRENT_SOURCE_DIR}/sub_dir"
target_include_directories(tcp_message PUBLIC "${embedded_cockpit_manager_SOURCE_DIR}/include")
target_include_directories(tcp PUBLIC "${embedded_cockpit_manager_SOURCE_DIR}/include")

# This depends on the following headers:
target_link_libraries(tcp_message PRIVATE ${HEADER_LIST})
target_link_libraries(tcp PRIVATE ${HEADER_LIST})

target_compile_features(tcp_message PUBLIC cxx_std_17)
target_compile_features(tcp PUBLIC cxx_std_17)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
