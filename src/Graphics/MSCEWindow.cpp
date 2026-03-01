#include "MSCEWindow.h"
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

MSCEWindow::MSCEWindow(int width, int height, const char *title) : p_window(glfwCreateWindow(width, height, title, nullptr, nullptr), &glfwDestroyWindow)
{
    this->width = width;
    this->height = height;
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
bool MSCEWindow::should_close()
{
    return glfwWindowShouldClose(this->p_window.get());
}

void MSCEWindow::swap_buffers()
{
    glfwSwapBuffers(this->p_window.get());
}