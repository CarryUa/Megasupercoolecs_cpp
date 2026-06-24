#src/MSCE/Platform/


if (CMAKE_SYSTEM_NAME STREQUAL "Linux") #linux sources
    target_sources(${MSCE_TARGET} 
    PRIVATE platform_linux.cpp
    )
# elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows") #windows sources
#     target_sources(${MSCE_TARGET} 
#     PRIVATE platform_windows.cpp
#     )
else()
    message(FATAL_ERROR "Unsoported OS: ${CMAKE_SYSTEM_TYPE}")
endif()