// This is a dummy file for CMake to determain language correctly when creating library.
// Also used for testing diferent parts of engine
#include <MSCE/msce.h>
using namespace msce;

int main(int argc, char **argv)
{
    static auto g_event_man = EventManager();
    static auto g_sys_man = SystemManager();
    static auto g_proto_man = PrototypeManager();
    static auto g_comp_man = ComponentManager();
    static auto g_enum_man = EnumManager();
    static auto g_ent_man = EntityManager();

    g_sys_man.init_all_systems();
    // static auto g_shader_man = ShaderManager();
    std::set_terminate(msce::internal::handle_terminate);

    srand(time(NULL));
    static auto g_logger = Logger("GLOBAL");
    g_logger.log_debug("Initializing all...");

    auto graphic_sys = g_sys_man.get_system<GraphicsSystem>();
    auto time_sys = g_sys_man.get_system<TimeSystem>();

    static auto root_window = graphic_sys->create_window();

    auto ent = EntityManager::instance->create_entity();

    auto t = ComponentManager::instance->create_component<TransformComponent>();
    t->shape = std::make_shared<Rectangle>(500, 500);
    auto r = ComponentManager::instance->create_component<BaseRendererComponent>();
    ent->attach_component(t);
    ent->attach_component(r);

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

        root_window->render();
        root_window->draw();
    }

    return 0;
}