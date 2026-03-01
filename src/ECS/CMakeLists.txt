add_library(libecs STATIC
            system_registry.hpp
            Managers/systemManager.cpp
            system.cpp
            component.cpp
            entity.cpp
            Common/Colors/rgba.cpp
            Graphics/MSCEWndow.cpp
            Common/vector.cpp
            )

target_link_libraries(libecs glad glfw)            
# Setup variables
set(ECS_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
            
# Export ECS_DIR to parent scope
set(ECS_DIR "${ECS_DIR}" PARENT_SCOPE)