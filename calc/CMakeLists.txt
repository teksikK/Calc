cmake_minimum_required(VERSION 3.30)
project(calc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===== Qt AUTOMOC (KRYTYCZNE) =====
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ===== Qt =====
set(CMAKE_PREFIX_PATH "C:/Qt/6.10.1/mingw_64")
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# ===== GUI =====
add_executable(calc_gui
        main.cpp
        MainWindow.cpp
        MainWindow.h
        calculator.cpp
)

target_link_libraries(calc_gui
        Qt6::Widgets
)

# ===== TESTY =====
add_executable(calc_tests
        calculator.cpp
        test_calculator.cpp
        catch_amalgamated.cpp
)
