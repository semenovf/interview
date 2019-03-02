cmake_minimum_required (VERSION 3.1)
project(cxxlang CXX)

set(EXE_TARGET cxxlang)

set(EXE_SOURCES
    main.cpp)

add_executable(${EXE_TARGET} ${EXE_SOURCES})

