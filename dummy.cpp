// This is a dummy file for CMake to determain language correctly when creating library.
// Also used for testing diferent parts of engine
#include <MSCE/msce.h>
using namespace msce;

int main(int argc, char **argv)
{
    std::set_terminate(msce::internal::handle_terminate);

    srand(time(NULL));
    static auto g_logger = Logger("GLOBAL");
    g_logger.log_debug("Initializing all...");
    static auto g_sys_man = SystemManager();
    g_sys_man.init_all_systems();
    static auto g_comp_man = ComponentManager();
    static auto g_proto_man = PrototypeManager();
    static auto g_enum_man = EnumManager();

    auto graphic_sys = g_sys_man.get_system<GraphicsSystem>();
    auto time_sys = g_sys_man.get_system<TimeSystem>();

    static auto root_window = graphic_sys->create_window();

    auto t = g_comp_man.create_component<TransformComponent>();
    t->shape = std::make_shared<Circle>(100);
    auto t2 = g_comp_man.create_component<TransformComponent>();
    t2->shape = std::make_shared<Triangle>(Triangle::create_sas(100, 90, 100));
    auto t3 = g_comp_man.create_component<TransformComponent>();
    t3->shape = std::make_shared<Rectangle>(100, 200);

    int frame = 0;
    double next_fps_time = 0.5;

    g_logger.log_debug("Starting main loop...");
    while (!root_window->should_close())
    {
        frame++;
        if (time_sys->get_total_millis() > next_fps_time)
        {
            g_logger.log_info("FPS: {}\tTimestamp: {}sec", frame * 3, time_sys->get_total_seconds());
            frame = 0;
            next_fps_time += 333;
        }
        g_sys_man.update_all_systems();

        t->position.x = sin(time_sys->get_total_seconds());
        t2->position.y = sin(time_sys->get_total_seconds());
        t3->scale.x = sin(time_sys->get_total_seconds());
        t3->scale.y = sin(time_sys->get_total_seconds());

        root_window->make_curent_context();
        glfwPollEvents();
        root_window->render();
        root_window->swap_buffers();
    }

    return 0;
}