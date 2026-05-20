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
    t->shape = std::make_shared<Circle>(100);
    auto t2 = gCompMan.create_component<TransformComponent>();
    t2->shape = std::make_shared<Triangle>(Triangle::create_sas(100, 90, 100));
    auto t3 = gCompMan.create_component<TransformComponent>();
    t3->shape = std::make_shared<Rectangle>(100, 200);

    int frame = 0;
    unsigned long next_fps_time = 0;

    while (!rootWindow->should_close())
    {
        frame++;
        if (timeSys->get_total_seconds() + 1 > next_fps_time)
        {
            gLogger.log_info("FPS: {}\tTimestamp: {}sec", frame, timeSys->get_total_seconds());
            frame = 0;
            next_fps_time++;
        }
        gSysMan.update_all_systems();

        t->position.x = sin(timeSys->get_total_seconds());
        t2->position.y = sin(timeSys->get_total_seconds());
        t3->scale.x = sin(timeSys->get_total_seconds());
        t3->scale.y = sin(timeSys->get_total_seconds());

        rootWindow->make_curent_context();
        glfwSwapInterval(0);
        glfwPollEvents();
        rootWindow->render();
        rootWindow->swap_buffers();
    }

    return 0;
}