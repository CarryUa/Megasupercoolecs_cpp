// This is a dummy file for CMake to determain language correctly when creating library.
// Also used for testing diferent parts of engine
#include <Managers/systemManager.h>
#include <BuiltInSystems/graphicsSystem.h>
#include <Graphics/MSCEWindow.h>
#include <iostream>
#include <string>
using namespace msce;

int main(int argc, char **argv)
{
    SystemManager sys_man = SystemManager();
    sys_man.init_all_systems();

    GraphicsSystem *graphics_sys = sys_man.get_system<GraphicsSystem>();
    TimeSystem *time = sys_man.get_system<TimeSystem>();
    cout << graphics_sys << endl;
    MSCEWindow *window = graphics_sys->create_window();
    MSCEWindow *window2 = graphics_sys->create_window(Vector2D<int>(300, 300));

    while (!window->should_close() || !window2->should_close())
    {
        glfwPollEvents();
        window->make_curent_context();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        window->swap_buffers();

        window2->make_curent_context();
        glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        window2->swap_buffers();
    }

    return 0;
}