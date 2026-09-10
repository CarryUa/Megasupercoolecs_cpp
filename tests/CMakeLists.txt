project(Megasupercoolecs_tests VERSION 0.1.0)

add_subdirectory(gtest)

if(BUILD_TESTING)
    # Add source files to executable
    add_executable(run_tests 
                main.cpp
                entity.cpp
                system.cpp
                component.cpp
                events.cpp
                prototypes.cpp
                reflection.cpp
                )

    target_link_libraries(run_tests GTest::gtest GTest::gtest_main ${MSCE_TARGET})
    target_include_directories(run_tests
                               PRIVATE gtest/include
                               PRIVATE src/MSCE
                               PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                               PRIVATE ${CMAKE_SOURCE_DIR}/include
                               )
endif()
