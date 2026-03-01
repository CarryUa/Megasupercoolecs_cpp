#include "MSCEWindow.h"
#include <iostream>
using namespace std;

MSCEWindow::MSCEWindow(int width, int height, const char *title)
{
    this->width = width;
    this->height = height;
    this->title = title;

    if (glfwInit() != GLFW_TRUE)
    {
        throw runtime_error("Failed to initialize GLFW");
    }

    p_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (p_window == nullptr)
    {
        glfwTerminate();
        throw runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(p_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        cerr << "Failed to initialize GLAD" << endl;
        throw runtime_error("Failed to initialize GLAD");
    }
}
MSCEWindow::~MSCEWindow()
{
    if (p_window != nullptr)
    {
        glfwDestroyWindow(p_window);
        p_window = nullptr;
    }
}