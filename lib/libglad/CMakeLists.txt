project(glad_project)
add_library(glad glad.c)

# set_target_properties(glad PROPERTIES
#                         POSITION_INDEPENDENT_CODE on)

target_include_directories(glad
                            PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})