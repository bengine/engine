project(engine C)
cmake_minimum_required(VERSION 3.4)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

add_subdirectory(core)
add_subdirectory(usaa)