#ifndef _MSCE_WINDOW_H_
#define _MSCE_WINDOW_H_
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <MSCE/Types/vector.h>
#include <memory>

#include <MSCE/BuiltIns/timeSystem.h>
#include <MSCE/Managers/componentManager.h>

using namespace std;

namespace msce
{
    class MSCEWindow
    {
        GLuint shader_program_;
        TimeSystem *t_sys_;
        ComponentManager *comp_man_ = nullptr;
        unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> p_window_;
        unsigned int vbo_, vao_;

    public:
        Vector2D<int> window_size;
        const char *title;
        MSCEWindow(Vector2D<int> window_size_ = Vector2D<int>(500, 500), const char *title = "Window", GLFWmonitor *glfw_monitor = nullptr, GLFWwindow *glfw_share = nullptr);
        bool should_close();
        void render();
        void swap_buffers();
        void make_curent_context();

        static void on_resize(GLFWwindow *, int w, int h);
    };
}

#endif //_MSCE_WINDOW_H_