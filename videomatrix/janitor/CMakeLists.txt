cmake_minimum_required(VERSION 3.1)
project(janitor CXX C)

find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
    config.cpp
    ../shared/advance.cpp)

target_link_libraries(${PROJECT_NAME} Threads::Threads)
target_include_directories(${PROJECT_NAME} PUBLIC ../shared)
