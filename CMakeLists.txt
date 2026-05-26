cmake_minimum_required(VERSION 3.20)
project(containerostream CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(containerostream INTERFACE)
target_include_directories(containerostream INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(containerostream INTERFACE cxx_std_23)

add_executable(test test.cpp)
target_link_libraries(test PRIVATE containerostream)
