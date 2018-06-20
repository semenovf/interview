#
# Based on:
# https://cmake.org/cmake/help/v3.0/module/FindQt4.html
#

cmake_minimum_required (VERSION 3.1)

project(partman CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt4 REQUIRED QtGui QtXml)

set(SOURCES
    src/main.cpp
    src/DiskListView.cpp
    src/DiskSummaryWidget.cpp
    src/GraphicalView.cpp
    src/GraphicalViewItem.cpp
    src/MainWindow.cpp
    src/Model.cpp
    src/TableView.cpp
    src/TestModel.cpp
    src/VolumeListView.cpp
    src/VolumeSummaryWidget.cpp)

add_executable(partman ${SOURCES})
target_link_libraries(partman Qt4::QtGui Qt4::QtXml)
