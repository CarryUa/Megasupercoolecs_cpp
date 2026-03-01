#include <iostream>
#include <ECS/system_registry.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ECS/system.h>
#include <ECS/Graphics/MSCEWindow.h>

using namespace std;
using namespace msce;

int main()
{
    register_all_systems();
    if (glfwInit() != GLFW_TRUE)
    {
        throw runtime_error("Failed to initialize GLFW");
    }

    SystemManager sysMan = SystemManager();

    sysMan.init_all_systems();

    MSCEWindow window = MSCEWindow();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    while (!window.should_close())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();
        window.swap_buffers();
    }

    glfwTerminate();

    return 0;
}