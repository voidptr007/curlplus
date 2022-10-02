# Set this project's name.
project("curlplus")

# Set the minimum required version of CMake for this project.
cmake_minimum_required(VERSION 3.8)

# Set the C++ standard to use to build this target.
set(CMAKE_CXX_STANDARD 20) 

#add project directory
add_subdirectory(tests)