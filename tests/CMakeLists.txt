project(Megasupercoolecs_tests VERSION 0.1.0)

add_subdirectory(gtest)

if(BUILD_TESTING)
    add_executable(Megasupercoolecs_tests tests.cpp)
    target_link_libraries(Megasupercoolecs_tests GTest::gtest GTest::gtest_main)
    target_include_directories(Megasupercoolecs_tests PRIVATE gtest/include)
endif()
