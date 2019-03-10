cmake_minimum_required(VERSION 3.1)
project(cxxlang CXX)
enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/../pfs/tests)
#
# Add unit test targets
#
set(MY_TEST_TARGETS 
    1.initialization_list
    2.rvalue
    3.constexpr
    4.variadic_template
    5.lambda
    6.decltype
    7.enum_class
)

# This is the "object library" target: compiles the sources only once
add_library(CATCH_OBJLIB OBJECT ${CMAKE_SOURCE_DIR}/../pfs/tests/catch-main.cpp)
# Optional: shared libraries need PIC
set_property(TARGET CATCH_OBJLIB PROPERTY POSITION_INDEPENDENT_CODE 1)
add_library(catch SHARED $<TARGET_OBJECTS:CATCH_OBJLIB>)

#
# Process all unit test targets
#
foreach (MY_TEST_TARGET ${MY_TEST_TARGETS})
    add_executable(${MY_TEST_TARGET} ${MY_TEST_TARGET}.cpp)
    target_link_libraries(${MY_TEST_TARGET} catch)
    add_test(NAME ${MY_TEST_TARGET} COMMAND ${MY_TEST_TARGET})
endforeach(MY_TEST_TARGET)
