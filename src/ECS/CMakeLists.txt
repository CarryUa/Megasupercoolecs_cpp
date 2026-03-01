# /src/ECS

target_sources(${MSCE_TARGET}
                PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/system.cpp
                PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/component.cpp
                PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/entity.cpp)

