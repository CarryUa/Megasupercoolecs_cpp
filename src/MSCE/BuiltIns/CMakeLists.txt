# /src/BuiltIns

target_sources(${MSCE_TARGET}
                PRIVATE timeSystem.cpp
                PRIVATE graphicsSystem.cpp
                PRIVATE inputSystem.cpp
                PRIVATE Renderers/baseRendererSystem.cpp
                PRIVATE Renderers/spriteRendererSystem.cpp
                )