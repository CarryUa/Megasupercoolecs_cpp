#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class MSCEWindow
{
private:
    GLFWwindow *window;

public:
    int width;
    int height;
    const char *title;
    MSCEWindow(int width = 500, int height = 500, const char *title = "MSCE Window");
    ~MSCEWindow();
    void DrawAll();
};