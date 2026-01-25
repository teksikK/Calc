cmake_minimum_required(VERSION 3.30)
project(calc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ---- Qt ----
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# ---- GUI executable ----
add_executable(calc_gui
        calc/main.cpp
        calc/MainWindow.cpp
        calc/MainWindow.h
        calc/calculator.cpp
)

target_link_libraries(calc_gui
        Qt6::Widgets
)

# ---- Tests (no Qt) ----
add_executable(calc_tests
        calc/calculator.cpp
        calc/test_calculator.cpp
        calc/catch_amalgamated.cpp
)