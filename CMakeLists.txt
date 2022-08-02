cmake_minimum_required(VERSION 3.21)
project(TinySmartPtr)

set(CMAKE_CXX_STANDARD 14)

add_executable(TinySmartPtr test.cpp smartPtr.cpp smartPtr.h)
