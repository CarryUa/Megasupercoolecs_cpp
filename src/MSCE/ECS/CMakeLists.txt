# /src/ECS

target_sources(${MSCE_TARGET}
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/system.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/component.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/entity.cpp)

