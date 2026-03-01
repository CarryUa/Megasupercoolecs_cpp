cmake_minimum_required(VERSION 3.28.0)
project(Megasupercoolecs VERSION 0.1.0 LANGUAGES CXX)
include(CTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_POSITION_INDEPENDENT_CODE on)

# definitions
set(MSCE_TARGET "libmsce")

add_library(${MSCE_TARGET} SHARED dummy.cpp)

# Subdirs
add_subdirectory(src)
add_subdirectory(src/BuiltInSystems)
add_subdirectory(src/Common)
add_subdirectory(src/Common/Colors)
add_subdirectory(src/ECS)
add_subdirectory(src/Graphics)
add_subdirectory(src/Managers)

add_subdirectory(tests)

add_subdirectory(lib/libglad)
add_subdirectory(lib/libglfw)

# Link libraries
target_link_libraries(${MSCE_TARGET}
                        glad
                        glfw)   

# Include dirs
target_include_directories(${MSCE_TARGET} PUBLIC src)
target_include_directories(${MSCE_TARGET} PRIVATE lib/libglfw/include)
