// This is a dummy file for CMake to determain language correctly when creating library.
// Also used for testing diferent parts of engine
#include <MSCE/Managers/systemManager.h>
#include <MSCE/Managers/componentManager.h>
#include <MSCE/BuiltInComponents/transformComponent.hpp>
#include <MSCE/BuiltInSystems/graphicsSystem.h>
#include <MSCE/Graphics/MSCEWindow.h>
#include <iostream>
#include <string>
using namespace msce;

int main(int argc, char **argv)
{
    SystemManager sys_man = SystemManager();

    ComponentManager comp_man = ComponentManager();
    sys_man.init_all_systems();

    GraphicsSystem *graphics_sys = sys_man.get_system<GraphicsSystem>();
    TimeSystem *time = sys_man.get_system<TimeSystem>();
    cout << graphics_sys << endl;
    MSCEWindow *window = graphics_sys->create_window();
    MSCEWindow *window2 = graphics_sys->create_window(Vector2D<int>(300, 300));

    TransformComponent *t1 = comp_man.create_component<TransformComponent>();
    TransformComponent *t2 = comp_man.create_component<TransformComponent>();

    t2->position.x = 10.0;

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