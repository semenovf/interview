cmake_minimum_required (VERSION 3.1)
project(calculator CXX)

set(EXE_TARGET calculator)
set(LIB_TARGET doit)
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
    Result.cpp
    Worker.cpp)

include_directories(${CMAKE_CURRENT_LIST_DIR})
add_executable(${EXE_TARGET} ${EXE_SOURCES})

add_library(${LIB_TARGET} SHARED DoIt.cpp)
set_property(TARGET ${LIB_TARGET} PROPERTY POSITION_INDEPENDENT_CODE 1)

target_link_libraries(${EXE_TARGET} Qt5::Widgets)
#target_link_libraries(${EXE_TARGET} Qt4::QtGui)

