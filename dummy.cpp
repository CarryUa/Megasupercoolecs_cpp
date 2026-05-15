// This is a dummy file for CMake to determain language correctly when creating library.
// Also used for testing diferent parts of engine
#include <MSCE/msce.h>
using namespace msce;

int main(int argc, char **argv)
{
    srand(time(NULL));
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
    auto timeSys = gSysMan.get_system<TimeSystem>();

    static auto rootWindow = graphicSys->create_window();

    auto t = gCompMan.create_component<TransformComponent>();
    t->position = vec2d(0, 0);
    t->shape = std::make_shared<Circle>();

    while (!rootWindow->should_close())
    {
        gLogger.log_info("DeltaT: {}", timeSys->get_delta_time());
        gSysMan.update_all_systems();

        t->scale.x = sin(timeSys->get_total_seconds() * 10) / 2 + 1;
        t->scale.y = sin(timeSys->get_total_seconds() * 10) / 2 + 1;

        rootWindow->make_curent_context();
        glfwPollEvents();
        rootWindow->render();
        rootWindow->swap_buffers();
    }

    return 0;
}