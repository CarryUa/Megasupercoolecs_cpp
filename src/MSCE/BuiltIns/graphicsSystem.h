#ifndef _MSCE_GRAPHICS_SYSTEM_H_
#define _MSCE_GRAPHICS_SYSTEM_H_
#include <MSCE/Managers/componentManager.h>
#include <MSCE/Graphics/MSCEWindow.h>
#include <MSCE/ECS/system.h>
#include <MSCE/Types/vector.h>
#include <vector>
#include <memory>
class GLFWmonitor;
class GLFWwindow;
namespace msce
{

    /// @brief Handles GL-related stuff. Uses several sub-systems like ShaderSystem, ect.
    class GraphicsSystem : public System
    {
    private:
        GLuint vertex_sh_;
        GLuint frag_sh_;
        GLuint program_;
        std::vector<std::unique_ptr<MSCEWindow>> windows_;
        ComponentManager *comp_man_ = nullptr;

    public:
        GraphicsSystem();
        void pre_rended_all_objects_on_window(MSCEWindow *);
        MSCEWindow *get_window(size_t id);
        MSCEWindow *create_window(Vector2D<int> window_size_ = Vector2D<int>(500, 500), const char *title = "Window", GLFWmonitor *glfw_monitor = nullptr, GLFWwindow *glfw_share = nullptr);
        // void destroy_window(MSCEWindow *window);
        void init() override;
    };
}
MSCE_REGISTER_SYSTEM(msce::GraphicsSystem, GraphicsSystem)

#endif //_MSCE_GRAPHICS_SYSTEM_H_