cmake_minimum_required(VERSION 3.8)
project(ParallelTreeSearch)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Tree.h TreeNode.h)
add_executable(ParallelTreeSearch ${SOURCE_FILES})