// This is a dummy file for CMake to determain language correctly when creating
// library. Also used for testing diferent parts of engine
#include <MSCE/BuiltIns/Renderers/spriteRendererComponent.hpp>
#include <MSCE/Types/image.h>
#include <MSCE/BuiltIns/inputSystem.h>

#include <MSCE/prototype.h>
#include <MSCE/event.h>
#include <MSCE/Types/enum.h>
#include <MSCE/terminator.h>

using namespace msce;

class TestSystem : public System
{
  SystemDependency<TimeSystem> timesys_;

  virtual void init()
  {
    Logger logger("adasd");
    logger.log_debug("DependencyInjected: {}", timesys_->active);
  }
};

MSCE_REGISTER_SYSTEM(TestSystem)

void log_key(KeyEventArgs &args)
{
  if (args.key.control_held() && args.key.keycode == GLFW_KEY_R)
  {
    SystemManager::instance->get_system<GraphicsSystem>()
        ->get_window(0)
        ->reload_shaders();
  }
  else if (args.key.keycode == GLFW_KEY_ESCAPE)
    exit(0);
}

void log_click(MouseButtonEventArgs &args)
{
  static Logger logger("MouseClick");

  logger.log_info("Mouse {} at: {}, {} (Button: {})",
                  args.action == InputAction::PRESSED ? "pressed"
                  : args.action == InputAction::HELD  ? "held"
                                                      : "released",
                  args.pos.x, args.pos.y,
                  args.button == MouseButton::LEFT_MB     ? "LMB"
                  : args.button == MouseButton::MIDDLE_MB ? "MMB"
                                                          : "RMB");
}

int main(int argc, char **argv)
{
  std::set_terminate(msce::internal::handle_terminate);

  static auto g_event_man = EventManager();
  static auto g_sys_man = SystemManager();
  static auto g_proto_man = PrototypeManager();
  static auto g_comp_man = ComponentManager();
  static auto g_enum_man = EnumManager();
  static auto g_ent_man = EntityManager();
  g_sys_man.init_all_systems();

  srand(time(NULL));
  static auto g_logger = Logger("GLOBAL");
  g_logger.log_debug("Initializing all...");

  auto graphic_sys = g_sys_man.get_system<GraphicsSystem>();
  auto time_sys = g_sys_man.get_system<TimeSystem>();

  static auto root_window = graphic_sys->create_window(vec2i(1280, 720));

  auto ent = EntityManager::instance->create_entity();
  auto trans =
      ComponentManager::instance->create_component<TransformComponent>();
  ent->attach_component(trans);

  auto rend =
      ComponentManager::instance->create_component<SpriteRendererComponent>();
  ent->attach_component(rend);

  trans->shape = std::make_shared<Rectangle>();

  rend->img = std::make_shared<Image>("./cmss14 yaoi vs yuri.psd.png");
  auto *img = rend->img.get();

  root_window->bind_image(*img);

  g_logger.log_info("Image bount: {}x{} at 0x{:X}", img->resolution.x,
                    img->resolution.y, reinterpret_cast<uintptr_t>(img));

  for (int i = 0; i < 1; ++i)
  {
    auto copy = EntityManager::instance->copy_entity(ent);
    copy->get_component<TransformComponent>()->position.x += 150;
    g_logger.log_info(
        "Has comp: {}; handle({},{}); addr: 0x{:x}",
        copy->has_component<SpriteRendererComponent>(),
        copy->get_component<SpriteRendererComponent>().get_index(),
        copy->get_component<SpriteRendererComponent>().get_generation(),
        reinterpret_cast<uintptr_t>(
            copy->get_component<SpriteRendererComponent>().get()));

    copy->get_component<SpriteRendererComponent>()->img = rend->img;
  }

  int frame = 0;
  double next_fps_time = 0.5;
  g_logger.log_debug("Starting main loop...");

  g_sys_man.get_system<InputSystem>()->on_keyboard_input.subscribe(log_key);
  g_sys_man.get_system<InputSystem>()->on_mouse_input.subscribe(log_click);

  while (!root_window->should_close())
  {
    frame++;
    if (time_sys->get_total_millis() > next_fps_time)
    {
      g_logger.log_info("FPS: {}\tTimestamp: {}sec", frame / 5,
                        time_sys->get_total_seconds());
      frame = 0;
      next_fps_time += 5000;
    }
    g_sys_man.update_all_systems();

    root_window->render();
    root_window->draw();
  }

  return 0;
}