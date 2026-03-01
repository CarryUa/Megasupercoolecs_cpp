cmake_minimum_required(VERSION 3.28.0)
project(Megasupercoolecs_project VERSION 0.1.0)
include(CTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

# Add source files
add_subdirectory(src)

add_executable(Megasupercoolecs
                src/main.cpp
                ${SOURCE_BUNDLE})

# ECS includes
target_link_libraries(Megasupercoolecs libecs)
target_include_directories(libecs PUBLIC ${ECS_DIR}/..)

# Link glad
add_subdirectory(lib/libglad)
target_link_libraries(Megasupercoolecs glad)

# Link glfw
add_subdirectory(lib/libglfw)
target_link_libraries(Megasupercoolecs glfw)
target_include_directories(Megasupercoolecs INTERFACE lib/libglfw/include)

# Tests
add_subdirectory(tests)