cmake_minimum_required(VERSION 3.1)
project(cxxlang CXX)
enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/../pfs/tests)
#
# Add unit test targets
#
set(MY_TEST_TARGETS
    001.initialization_list
    002.rvalue
    003.constexpr
    004.variadic_template
    005.lambda
    006.decltype
    007.enum_class
    008.smart_ptr
    009.algo.1.nonmod
    009.algo.2.mod
    009.algo.3.del
    009.algo.4.permut
    009.algo.5.sort
    009.algo.6.order
    009.algo.7.num
    010.thread
    sort.impl
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
    target_link_libraries(${MY_TEST_TARGET} catch pthread)

    if (CMAKE_COMPILER_IS_GNUCXX STREQUAL "GNU"
            AND ${MY_TEST_TARGET} MATCHES "thread")
        target_link_libraries(${MY_TEST_TARGET} pthread)
    endif()

    add_test(NAME ${MY_TEST_TARGET} COMMAND ${MY_TEST_TARGET})
endforeach(MY_TEST_TARGET)
