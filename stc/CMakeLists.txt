cmake_minimum_required (VERSION 3.1)
project(calculator CXX)

set(EXE_TARGET calculator)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED Widgets)
#find_package(Qt4 REQUIRED QtGui)

set(EXE_SOURCES
    main.cpp
    MainWindow.cpp
    Console.cpp
    Operation.cpp
    Worker.cpp
    DoIt.cpp)

include_directories(${CMAKE_CURRENT_LIST_DIR})
add_executable(${EXE_TARGET} ${EXE_SOURCES})

target_link_libraries(${EXE_TARGET} Qt5::Widgets)
#target_link_libraries(${EXE_TARGET} Qt4::QtGui)

