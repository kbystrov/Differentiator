cmake_minimum_required(VERSION 3.12)
project(Differentiator)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 14)

add_executable(Differentiator main.cpp TreeUtils/Tree.cpp TreeUtils/Tree.h Common/data_types.h TreeUtils/Node.cpp TreeUtils/Node.h Common/err_codes.h TestUtils/test_utils.cpp TestUtils/test_utils.h Common/variables.h Common/operators.h ReadWriteUtils/read_write_utils.cpp ReadWriteUtils/read_write_utils.h DifferentiatorUtils/differentiator_utils.cpp DifferentiatorUtils/differentiator_utils.h)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall")