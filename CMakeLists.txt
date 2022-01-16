cmake_minimum_required(VERSION 3.21)
project(sqlite-o)

set(CMAKE_CXX_STANDARD 14)

add_library(${PROJECT_NAME} src/SQLiteO.cpp)
