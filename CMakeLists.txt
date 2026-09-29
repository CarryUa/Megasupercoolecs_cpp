cmake_minimum_required(VERSION 3.28.0)
project(Megasupercoolecs VERSION 0.1.0 LANGUAGES CXX)

include(ExternalProject)

if(WIN32)
  add_compile_definitions(_USE_MATH_DEFINES)
else()

  # Colorfull output
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-fcolor-diagnostics)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(-fdiagnostics-color=always)
  endif()
endif()

set(DOXYGEN_EXECUTABLE "" FORCE)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DRELEASE")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DDEBUG")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "-g")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-g")
set(CMAKE_CXX_STANDARD 20)

add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Wno-invalid-offsetof>)
add_compile_options(-Wno-pointer-arith )

if(NOT MSCE_SKIP_TESTS)
    include(CTest)
endif ()




set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_POSITION_INDEPENDENT_CODE on)


# Build Versioning
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "CMAKE BUILD TYPE IS ${CMAKE_BUILD_TYPE}")

# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
# endif()


# definitions
set(MSCE_TARGET "msce")

add_library(${MSCE_TARGET} SHARED)

# Subdirs
add_subdirectory(src)
add_subdirectory(src/Platform)
add_subdirectory(src/BuiltIns)
add_subdirectory(src/Types)
add_subdirectory(src/ABI)



add_subdirectory(tests)

add_executable(run dummy.cpp)
target_link_libraries(run ${MSCE_TARGET})
target_include_directories(run PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(lib/libglad)
add_subdirectory(lib/libglfw)
add_subdirectory(lib/boost/preprocessor)

set(ZLIB_BUILD_STATIC off CACHE BOOL "" FORCE)
set(ZLIB_BUILD_SHARED on CACHE BOOL "" FORCE)
set(ZLIB_BUILD_TESTING off CACHE BOOL "" FORCE)
add_subdirectory(lib/zlib)
set(ZLIB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/zlib" CACHE PATH "" FORCE)
set(ZLIB_LIBRARY "$<TARGET_FILE:zlib>" CACHE FILEPATH "" FORCE)


set(PNG_STATIC on CACHE BOOL "" FORCE)
set(PNG_SHARED off CACHE BOOL "" FORCE)
add_subdirectory(lib/libpng)


set(JPEG_PREFIX ${CMAKE_BINARY_DIR}/lib/libjpeg)
if(MSVC)
  set(JPEG_LIB ${JPEG_PREFIX}/lib/jpeg-static${CMAKE_STATIC_LIBRARY_SUFFIX})
else()
  set(JPEG_LIB ${JPEG_PREFIX}/lib/libjpeg${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()

ExternalProject_Add(libjpeg_ext
SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/libjpeg
CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${JPEG_PREFIX}
    -DCMAKE_INSTALL_LIBDIR=lib
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON
    -DENABLE_SHARED=OFF
    -DENABLE_STATIC=ON
    -DWITH_TURBOJPEG=OFF
BUILD_BYPRODUCTS ${JPEG_LIB}
)

set(BUILD_DOC OFF CACHE BOOL "" FORCE)
set(BUILD_SANDBOX OFF CACHE BOOL "" FORCE)
set(SKIP_PERFORMANCE_COMPARISON ON CACHE BOOL "" FORCE)
set(WITH_WERROR OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/cereal)


# Link libraries
target_link_libraries(${MSCE_TARGET}
                        glad
                        glfw
                        cereal
                        boost_preprocessor
                        dl
                        dw
                        zlib
                        png_static
                        jpeg
                        )    

# Include dirs
target_include_directories(${MSCE_TARGET} PUBLIC src
                          PRIVATE ${CMAKE_SOURCE_DIR}/include
                          PRIVATE src/MSCE
                          PRIVATE lib/boost/include
                          PRIVATE lib/libglfw/include
                          PRIVATE lib/libglfw/include
                          PUBLIC lib/cereal/include
                          PUBLIC lib/libjpeg/src
                          PUBLIC lib
                          )
