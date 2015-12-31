project(engine C)
cmake_minimum_required(VERSION 3.4)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

option(BUILD_SHARED_LIBS "Build all libraries to be shared" ON)

add_subdirectory(base64)
add_subdirectory(yaml)
