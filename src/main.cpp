#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ECS/System/system.h>
#include <ECS/Graphics/MSCEWindow/MSCEWindow.h>

using namespace std;

int main()
{
    MSCEWindow window = MSCEWindow();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    while (!glfwWindowShouldClose(window.p_window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();
        glfwSwapBuffers(window.p_window);
    }

    glfwTerminate();

    return 0;
}