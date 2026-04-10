cmake_minimum_required(VERSION 3.28.0)
project(Megasupercoolecs VERSION 0.1.0 LANGUAGES CXX)
include(CTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_POSITION_INDEPENDENT_CODE on)


# Build Versioning
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
message(STATUS "CMAKE BUILD TYPE IS ${CMAKE_BUILD_TYPE}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(MSCE_BUILD_TYPE_DEBUG)
else()
    add_compile_definitions(MSCE_BUILD_TYPE_RELEASE)
endif()


# definitions
set(MSCE_TARGET "msce")

add_library(${MSCE_TARGET} SHARED dummy.cpp)

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
add_subdirectory(src/MSCE/Prototypes)

add_subdirectory(tests)

add_subdirectory(lib/libglad)
add_subdirectory(lib/libglfw)
add_subdirectory(lib/cereal)
add_subdirectory(lib/boost/preprocessor)

# Link libraries
target_link_libraries(${MSCE_TARGET}
                        glad
                        glfw
                        cereal
                        boost_preprocessor)   


# Include dirs
target_include_directories(${MSCE_TARGET} PUBLIC src)
target_include_directories(${MSCE_TARGET} PRIVATE src/MSCE)
target_include_directories(${MSCE_TARGET} PUBLIC lib/cereal/include)
target_include_directories(${MSCE_TARGET} PRIVATE lib/boost/include)
target_include_directories(${MSCE_TARGET} PRIVATE lib/libglfw/include)
