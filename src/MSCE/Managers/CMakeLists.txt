# src/Managers

target_sources(${MSCE_TARGET}
                PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/systemManager.cpp
                PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/componentManager.cpp
                PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/entityManager.cpp
                PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/prototypeManager.cpp
                )