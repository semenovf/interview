cmake_minimum_required (VERSION 3.1)
project(qml-demo CXX)

set(EXE_TARGET qml-demo)

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
set(QT_MIN_VERSION "5.4.0")
find_package(Qt5 COMPONENTS Widgets Qml Quick REQUIRED)
qt5_add_resources(QT_RESOURCES qml-demo.qrc)

set(EXE_SOURCES
    main.cpp
#    MainWindow.cpp
#    Console.cpp
#    Operation.cpp
#    Result.cpp
#    Worker.cpp
)

#
# For translation
#
# file(GLOB TS_FILES "${PROJECT_SOURCE_DIR}/*.ts")
# qt5_create_translation(QM_FILES
#     ${PROJECT_SOURCE_DIR}
#     ${TS_FILES}
#     OPTIONS -source-language en_US -no-obsolete)

# Material specific configuration
add_definitions("-DQPM_INIT\\(E\\)=E.addImportPath\\(QStringLiteral\\(\\\"qrc:/\\\"\\)\\)\\;")
include(../3rdparty/material/vendor.cmake)

add_executable(${EXE_TARGET} ${EXE_SOURCES} ${QT_RESOURCES})
target_link_libraries(${EXE_TARGET} Qt5::Widgets Qt5::Qml Qt5::Quick)
