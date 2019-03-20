cmake_minimum_required (VERSION 3.1)
project(calculator CXX)

set(EXE_TARGET calculator)
set(LIB_TARGET doit)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${CMAKE_CURRENT_LIST_DIR})

# find_package(Qt5 REQUIRED Widgets)
# find_package(Qt5 COMPONENTS Quick Core)
# find_package(Qt5QuickCompiler)
# qtquick_compiler_add_resources(QT_RESOURCES calculator.qrc)
# qt5_use_modules(EXE_TARGET Quick Core)

find_package(Qt5 COMPONENTS Widgets Qml Quick REQUIRED)
qt5_add_resources(QT_RESOURCES calculator.qrc)

set(EXE_SOURCES
    main.cpp
#    MainWindow.cpp
#    Console.cpp
#    Operation.cpp
#    Result.cpp
#    Worker.cpp
)

add_executable(${EXE_TARGET} ${EXE_SOURCES} ${QT_RESOURCES})
target_link_libraries(${EXE_TARGET} Qt5::Widgets Qt5::Qml Qt5::Quick)

add_library(${LIB_TARGET} SHARED DoIt.cpp)
set_property(TARGET ${LIB_TARGET} PROPERTY POSITION_INDEPENDENT_CODE 1)
