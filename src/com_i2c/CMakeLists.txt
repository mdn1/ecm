#Add all includes
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${embedded_cockpit_manager_SOURCE_DIR}/include/*.hpp")

#Path relative to this file where the files are located. E.g. gpio_board.cpp -> ./gpio_board.cpp
add_library(gpio_board gpio_board.cpp ${HEADER_LIST} )
add_library(pcf8574_io pcf8574_io.cpp ${HEADER_LIST} )

#If any of the source files from the libraries above are in a subdirectory relative to this files directory, 
#simply add it to the line below. E.g. "${CMAKE_CURRENT_SOURCE_DIR}/sub_dir"
target_include_directories(gpio_board PUBLIC "${embedded_cockpit_manager_SOURCE_DIR}/include")
target_include_directories(pcf8574_io PUBLIC "${embedded_cockpit_manager_SOURCE_DIR}/include")

# This depends on the following headers:
target_link_libraries(gpio_board PRIVATE ${HEADER_LIST})
target_link_libraries(pcf8574_io PRIVATE ${HEADER_LIST})

target_compile_features(gpio_board PUBLIC cxx_std_17)
target_compile_features(pcf8574_io PUBLIC cxx_std_17)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
