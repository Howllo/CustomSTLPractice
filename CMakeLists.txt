cmake_minimum_required(VERSION 3.28)
project(Test)

set(CMAKE_CXX_STANDARD 26)

add_executable(Test main.cpp
        Standard/VecArray.h
        Test/Testing.cpp
        Test/Testing.h
        Standard/List.h)
