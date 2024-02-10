# Preamble
cmake_minimum_required(VERSION 3.11)
project(mafsrv VERSION 0.5 LANGUAGES CXX)

# Project wide setup
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dependencies
add_subdirectory(dep)

# Main targets
add_subdirectory(src)
