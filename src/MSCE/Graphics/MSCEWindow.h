#ifndef _MSCE_WINDOW_H_
#define _MSCE_WINDOW_H_
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

#include <MSCE/Types/vector.h>
#include <MSCE/BuiltIns/timeSystem.h>
#include <MSCE/Managers/componentManager.h>
#include <MSCE/Graphics/shaders.h>
#include <MSCE/BuiltIns/defaultShader.hpp>
#include <MSCE/Events/prototypeEvents.h>

using namespace std;

namespace msce
{
    class MSCEWindow
    {
    private:
        struct GLContext
        {
            std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
            uint vao = 0, vbo = 0;
        } context_;

        const Logger logger_;
        const std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> p_window_;
        std::string title_;
        vec2ui size_;

        void prepare_shaders();
        void use_context();

    public:
        MSCEWindow(vec2ui window_size_ = vec2ui(1600, 900), const char *title = "msce", GLFWmonitor *glfw_monitor = nullptr, GLFWwindow *glfw_share = nullptr);
        vec2ui size();
        void size(vec2ui s);

        bool should_close();

        void render();

        void draw();
    };
}

#endif //_MSCE_WINDOW_H_