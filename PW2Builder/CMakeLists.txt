cmake_minimum_required(VERSION 3.10)
project(PW2Builder)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)

# Add the executable target
add_executable(PW2Builder Builder.cpp)
