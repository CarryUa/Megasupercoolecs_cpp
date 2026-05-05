// This is a dummy file for CMake to determain language correctly when creating library.
// Also used for testing diferent parts of engine
#include <MSCE/msce.h>
using namespace msce;

int main(int argc, char **argv)
{
    static auto gLogger = Logger("GLOBAL");
    gLogger.log_info("STARTING...");
    gLogger.log_info("Initializing SystemManager...");
    static auto gSysMan = SystemManager();
    gSysMan.init_all_systems();
    gLogger.log_info("Initializing ComponentManager...");
    static auto gCompMan = ComponentManager();
    gLogger.log_info("Initializing PrototypeManager...");
    static auto gProtoMan = PrototypeManager();

    auto graphicSys = gSysMan.get_system<GraphicsSystem>();

    static auto rootWindow = graphicSys->create_window();

    while (!rootWindow->should_close())
    {
        rootWindow->make_curent_context();
        glfwPollEvents();
        rootWindow->swap_buffers();
    }

    return 0;
}