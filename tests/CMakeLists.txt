# Set the C++ standard to use to build this target.
set(CMAKE_CXX_STANDARD 20) 

#include headers from include folder
include_directories(
        ${PROJECT_SOURCE_DIR}/include
        )

#define global variable for all cpp files
file(GLOB CURLPLUS_SOURCES
        "${PROJECT_SOURCE_DIR}/tests/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        )

#set exe dir
set(EXECUTABLE_OUTPUT_PATH "..")

#compile and build exe for source files
add_executable(curlplus_test ${CURLPLUS_SOURCES})

#link libraries
target_link_libraries(curlplus_test pthread)
target_link_libraries(curlplus_test curl)
target_link_libraries(curlplus_test stdc++fs)


