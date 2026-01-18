cmake_minimum_required(VERSION 3.30)
project(calc)

set(CMAKE_CXX_STANDARD 20)

add_executable(calc
        calculator.cpp
        calculator.h
        test_calculator.cpp
        catch_amalgamated.cpp)
