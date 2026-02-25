#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ECS/System/system.h>

using namespace std;

int main()
{
    if (glfwInit() != GLFW_TRUE)
    {
        cerr << "Failed to initialize GLFW" << endl;
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(800, 600, "Megasupercoolecs", nullptr, nullptr);
    if (window == nullptr)
    {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        cerr << "Failed to initialize GLAD" << endl;
        return -1;
    }
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}