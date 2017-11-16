cmake_minimum_required(VERSION 3.5)
project(ParallelTreeSearch)

set(CMAKE_CXX_STANDARD 11)

find_package (Threads)

set(SOURCE_FILES main.cpp Tree.h TreeNode.h Monitor.h)
add_executable(ParallelTreeSearch ${SOURCE_FILES})
target_link_libraries (ParallelTreeSearch ${CMAKE_THREAD_LIBS_INIT})
