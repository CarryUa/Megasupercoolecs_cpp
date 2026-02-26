add_library(libecs STATIC
            System/system.cpp
            Component/component.cpp
            Entity/entity.cpp
            Graphics/Colors/rgba.cpp
            Graphics/MSCEWindow/MSCEWndow.cpp
            Graphics/Vectors/vector.cpp)

target_link_libraries(libecs glad glfw)            
# Setup variables
set(ECS_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
            
# Export ECS_DIR to parent scope
set(ECS_DIR "${ECS_DIR}" PARENT_SCOPE)