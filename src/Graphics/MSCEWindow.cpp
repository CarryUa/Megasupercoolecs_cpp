#include "MSCEWindow.h"
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;
using namespace msce;

MSCEWindow::MSCEWindow(Vector2D<int> window_size_, const char *title, GLFWmonitor *glfw_monitor, GLFWwindow *glfw_share) : p_window(glfwCreateWindow(window_size_.x, window_size_.y, title, glfw_monitor, glfw_share), &glfwDestroyWindow)
{
    this->window_size = Vector2D(window_size_);
    this->title = title;

    if (p_window.get() == nullptr)
    {
        glfwTerminate();

        throw runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(p_window.get());

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        throw runtime_error("Failed to initialize GLAD");
    }
}

void MSCEWindow::make_curent_context()
{
    glfwMakeContextCurrent(p_window.get());
}

bool MSCEWindow::should_close()
{
    return glfwWindowShouldClose(this->p_window.get());
}

void MSCEWindow::swap_buffers()
{
    glfwSwapBuffers(this->p_window.get());
}