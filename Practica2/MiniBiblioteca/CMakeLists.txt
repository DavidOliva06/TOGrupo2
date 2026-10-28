cmake_minimum_required(VERSION 3.16)
project(MiniBiblioteca LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable(MiniBiblioteca
    main.cpp
    biblioteca.cpp biblioteca.h
    ventana.cpp ventana.h
    excepciones.h
)
target_link_libraries(MiniBiblioteca PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
