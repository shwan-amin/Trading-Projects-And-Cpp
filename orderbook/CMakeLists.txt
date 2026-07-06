cmake_minimum_required(VERSION 3.20)
project(OrderBook VERSION 1.0 LANGUAGES CXX)
 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
 
# Compiler warnings
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()
 
# Collect all source files in src/
file(GLOB_RECURSE SOURCES src/*.cpp)
 
add_executable(order_book ${SOURCES})
 
target_include_directories(order_book PRIVATE include)