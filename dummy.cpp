// This is a dummy file for CMake to determain language correctly when creating library.
// Also used for testing diferent parts of engine
#include <Managers/systemManager.h>
#include <Graphics/MSCEWindow.h>
#include <iostream>

using namespace msce;

int main(int argc, char **argv)
{
    if (!glfwInit())
        return -1;

    auto start = std::chrono::high_resolution_clock::now();
    SystemManager sys_man = SystemManager();
    sys_man.init_all_systems();

    TimeSystem *time_sys = sys_man.get_system<TimeSystem>();

    MSCEWindow window = MSCEWindow();
    while (!window.should_close())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        sys_man.update_all_systems();

        glfwPollEvents();
        std::cout << std::setprecision(9) << "Total runtime: " << time_sys->get_total_seconds() << "s | delta: " << time_sys->get_delta_time() << "s" << std::endl;
        window.swap_buffers();
    }

    return 0;
}