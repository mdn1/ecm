### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

# Works with 3.18.4 and tested through 3.18.4
cmake_minimum_required(VERSION 3.18.4)

project(
    embedded_cockpit_manager 
    VERSION 0.1
    DESCRIPTION "This software manages the Raspberry pi, which connects the sojuz cockpits input and outputs with the central simulator pc."
    LANGUAGES CXX)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


# Only do these, if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # Todo: set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here.

    # Ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Nicely support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Testing only available if this is the main app.
    # Note: this needs to be done in the main CMakeLists
    # since it calls enable_testing, which must be in the
    # main CMakeLists.
    # Todo: implement test.
    # include(CTest)

    # Docs only available if this is the main app
    # Todo: Doxygen docu.
    # find_package(Doxygen)
    # if(Doxygen_FOUND)
    #     add_subdirectory(docs)
    # else()
    #     message(STATUS "Doxygen not found, not building docs")
    # endif()
endif()
  

#Name of the directory relative to this file where the sub CMakeLists.txt is located.
#Keep order in which the subdirectories should be added based on their dependencies (least dependent first).
# The compiled library codes are here
add_subdirectory(src/com_i2c)
add_subdirectory(src/com_tcp)

# The executable code is here
add_subdirectory(apps)

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
# Todo: implement test.
# if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
#    AND BUILD_TESTING)
#   add_subdirectory(tests)
# endif()
