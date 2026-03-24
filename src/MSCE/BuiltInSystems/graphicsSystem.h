#ifndef _MSCE_GRAPHICS_SYSTEM_H_
#define _MSCE_GRAPHICS_SYSTEM_H_
#include <MSCE/Graphics/MSCEWindow.h>
#include <MSCE/ECS/system.h>
#include <MSCE/Common/vector.h>
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
        std::vector<std::unique_ptr<MSCEWindow>> _windows;

    public:
        void select_window(size_t id);
        void select_window(MSCEWindow *window);
        MSCEWindow *get_window(size_t id);
        MSCEWindow *create_window(Vector2D<int> window_size_ = Vector2D<int>(500, 500), const char *title = "Window", GLFWmonitor *glfw_monitor = nullptr, GLFWwindow *glfw_share = nullptr);
        void init() override;
    };
}

#endif //_MSCE_GRAPHICS_SYSTEM_H_