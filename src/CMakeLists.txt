# The name of our project is "kgame". CMakeLists files in this project can 
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and 
# to the root binary directory of the project as ${HELLO_BINARY_DIR}. 
cmake_minimum_required(VERSION 3.10)

#include(cppcheck.cmake)
project (kgame)

add_subdirectory (zone_svr)
