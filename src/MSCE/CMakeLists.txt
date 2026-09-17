# /src
target_sources(${MSCE_TARGET}
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/msce_macros.h
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/logger.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/component.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/entity.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/system.cpp
                )