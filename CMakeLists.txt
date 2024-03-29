cmake_minimum_required(VERSION 3.10)
project(MatrixMultiplication)

set(CMAKE_CXX_STANDARD 11)


find_package(pika REQUIRED)
add_executable(matrix_multiply src/main.cpp src/Matrix.hpp)
target_link_libraries(matrix_multiply PRIVATE pika::pika)
