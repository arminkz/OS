cmake_minimum_required(VERSION 3.5)
project(ParallelTreeSearch)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Tree.h TreeNode.h Monitor.h)
add_executable(ParallelTreeSearch ${SOURCE_FILES})
