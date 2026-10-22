cmake_minimum_required(VERSION 3.28.0)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

project(editor VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DRELEASE")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DDEBUG")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "-g")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-g")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_POSITION_INDEPENDENT_CODE on)


add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE backend/main.cpp)

add_subdirectory(lib/httplib)
add_subdirectory(lib/nlohmannjson)



target_link_libraries(${PROJECT_NAME} PRIVATE msce
                                      PRIVATE httplib
                                      PRIVATE ${NLOHMANN_JSON_TARGET_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE ../include
                                           PRIVATE lib/httplib
                                           PRIVATE lib/nlohmannjson/include)
