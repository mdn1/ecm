# # Testing library
# FetchContent_Declare(
#   FC #fetch content
#   GIT_REPOSITORY https://github.com/XYZ
#   GIT_TAG ABC)
# FetchContent_MakeAvailable(FC)
# # Adds FC::FC

# # Tests need to be added as executables first
# add_executable(testlib testlib.cpp)

# # Using C++17 in the test
# target_compile_features(testlib PRIVATE cxx_std_17)

# # Should be linked to the main library, as well as the FC testing library
# target_link_libraries(testlib PRIVATE modern_library FC:FC)

# # If you register a test, then ctest and make test will run it.
# # You can also run examples and check the output, as well.
# add_test(NAME testlibtest COMMAND testlib) # Command can be a target
