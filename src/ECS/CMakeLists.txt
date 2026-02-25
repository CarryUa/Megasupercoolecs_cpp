add_library(libecs STATIC
            System/system.cpp
            Component/component.cpp
            Entity/entity.cpp)

            
            
# Setup variables
set(ECS_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
            
# Export ECS_DIR to parent scope
set(ECS_DIR "${ECS_DIR}" PARENT_SCOPE)