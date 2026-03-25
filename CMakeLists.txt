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
add_executable(run dummy.cpp)

# Subdirs
add_subdirectory(src/MSCE)
add_subdirectory(src/MSCE/BuiltInComponents)
add_subdirectory(src/MSCE/BuiltInSystems)
add_subdirectory(src/MSCE/Common)
add_subdirectory(src/MSCE/Common/Colors)
add_subdirectory(src/MSCE/Common/Interfaces)
add_subdirectory(src/MSCE/ECS)
add_subdirectory(src/MSCE/Graphics)
add_subdirectory(src/MSCE/Managers)

add_subdirectory(tests)

add_subdirectory(lib/libglad)
add_subdirectory(lib/libglfw)
add_subdirectory(lib/libyaml-cpp)

# Link libraries
target_link_libraries(${MSCE_TARGET}
                        glad
                        glfw
                        yaml-cpp)   

target_link_libraries(run ${MSCE_TARGET})

# Include dirs
target_include_directories(${MSCE_TARGET} PUBLIC src)
target_include_directories(${MSCE_TARGET} PRIVATE src/MSCE)
target_include_directories(run PUBLIC src/MSCE)
target_include_directories(${MSCE_TARGET} PRIVATE lib/libglfw/include)
