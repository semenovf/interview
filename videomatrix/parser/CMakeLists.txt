cmake_minimum_required(VERSION 3.1)
project(parser CXX)

add_executable(${PROJECT_NAME}
    main.cpp
    pattern.cpp
    proto.cpp
    ../shared/advance.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ../shared)
