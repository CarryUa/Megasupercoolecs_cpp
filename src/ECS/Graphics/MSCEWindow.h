#ifndef _MSCE_WINDOW_H_
#define _MSCE_WINDOW_H_
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

using namespace std;

class MSCEWindow
{
    unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> p_window;

public:
    int width;
    int height;
    const char *title;
    MSCEWindow(int width = 500, int height = 500, const char *title = "MSCE Window");
    bool should_close();
    void swap_buffers();
};

#endif //_MSCE_WINDOW_H_