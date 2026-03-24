#ifndef _MSCE_WINDOW_H_
#define _MSCE_WINDOW_H_
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <MSCE/Common/vector.h>
#include <memory>

using namespace std;

namespace msce
{
    class MSCEWindow
    {
        unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> p_window;

    public:
        Vector2D<int> window_size;
        const char *title;
        MSCEWindow(Vector2D<int> window_size_ = Vector2D<int>(500, 500), const char *title = "Window", GLFWmonitor *glfw_monitor = nullptr, GLFWwindow *glfw_share = nullptr);
        bool should_close();
        void swap_buffers();
        void make_curent_context();
    };
}

#endif //_MSCE_WINDOW_H_