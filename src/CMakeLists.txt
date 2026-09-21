# /src
target_sources(${MSCE_TARGET}
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/logger.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/reflection.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/terminator.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/shader.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/MSCEWindow.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/component.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/componentManager.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/entity.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/entityManager.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/system.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/systemManager.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/prototypeManager.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/enumManager.cpp
                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/eventManager.cpp
                )