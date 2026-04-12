# src/Managers

target_sources(${MSCE_TARGET}
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/systemManager.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/componentManager.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/entityManager.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/prototypeManager.cpp
                )