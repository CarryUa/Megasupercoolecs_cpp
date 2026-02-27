project(Megasupercoolecs_tests VERSION 0.1.0)

add_subdirectory(gtest)

if(BUILD_TESTING)
    # Add source files to executable
    add_executable(Megasupercoolecs_tests 
                main.cpp
                system_registry.cpp
                ECS/Graphics/Colors/rgba.cpp
                ECS/Graphics/Vectors/vector.cpp
                ECS/System/system.cpp
                )

    target_link_libraries(Megasupercoolecs_tests GTest::gtest GTest::gtest_main libecs)
    target_include_directories(Megasupercoolecs_tests
                               PRIVATE gtest/include
                               PRIVATE ${ECS_DIR}/..)
endif()
