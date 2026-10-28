cmake_minimum_required(VERSION 3.16)
project(ConversorNumero LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable(ConversorNumero main.cpp)
target_link_libraries(ConversorNumero PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
