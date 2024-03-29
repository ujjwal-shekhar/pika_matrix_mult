cmake_minimum_required(VERSION 3.10)
project(MatrixMultiplication)

set(CMAKE_CXX_STANDARD 11)

add_executable(matrix_multiply main.cpp Matrix.hpp)

find_package(pika REQUIRED)
add_executable(app main.cpp)
target_link_libraries(app PRIVATE pika::pika)
