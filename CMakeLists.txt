cmake_minimum_required(VERSION 3.16)
project(MiniSearchEngine)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(MiniSearchEngine
    main.cpp
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
)

target_link_libraries(MiniSearchEngine PRIVATE Qt6::Widgets)
