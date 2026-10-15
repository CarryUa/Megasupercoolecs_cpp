// This is a dummy file for CMake to determain language correctly when creating
// library. Also used for testing diferent parts of engine
#include <MSCE/msce.h>

#include <MSCE/physics.h>
#include <MSCE/BuiltIns/Renderers/spriteRendererComponent.hpp>
#include <MSCE/BuiltIns/Renderers/spriteRendererSystem.h>

#include <MSCE/Types/image.h>
#include <MSCE/BuiltIns/inputSystem.h>

#include <MSCE/Types/enum.h>
#include <MSCE/serialization.h>
#include <MSCE/BuiltIns/entityPrototype.hpp>
#include <MSCE/reflection.h>
#include <MSCE/Types/animation.h>

using namespace msce;

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
  msce::init_engine();

  srand(time(NULL));
  static auto g_logger = Logger("GLOBAL");
  g_logger.log_debug("Initializing all...");

  auto graphic_sys =
      msce::SystemManager::instance->get_system<GraphicsSystem>();
  auto time_sys = msce::SystemManager::instance->get_system<TimeSystem>();

  static auto root_window = graphic_sys->create_window(vec2i(1280, 720));

  int frame = 0;
  double next_fps_time = 0.5;

  msce::SystemManager::instance->get_system<InputSystem>()
      ->on_keyboard_input.subscribe(log_key);
  msce::SystemManager::instance->get_system<InputSystem>()
      ->on_mouse_input.subscribe(log_click);

  // auto ent = msce::EntityManager::instance->create_entity();
  // ent->attach_component(g_comp_man.create_component<TransformComponent>());
  // auto sprite = g_comp_man.create_component<SpriteRendererComponent>();
  // ent->attach_component(sprite);
  // sprite->img_path = "./aot.jpg";

  // msce::PrototypeManager::instance->create_new_prototype_instance(
  //     typeof(EntityPrototype).get_name_str(), "testentity");
  // msce::PrototypeManager::instance->get_prototype<EntityPrototype>("testentity")->entity
  // = ent;

  // msce::PrototypeManager::instance->serialize_prototype("testent2.msceproto",
  // "testentity");
  auto &ent_proto2 =
      *msce::PrototypeManager::instance->get_prototype<EntityPrototype>(
          "testentity");

  auto ent = msce::EntityManager::instance->copy_entity(ent_proto2.entity);
  ent->active = true;
  ent->get_component<PhysicsBodyComponent>()->body_type =
      PhysicsBodyType::STATIC;
  ent->get_component<TransformComponent>()->position.y -= 200;

  auto ent2 = msce::EntityManager::instance->copy_entity(ent_proto2.entity);
  ent2->active = true;
  // ent2->get_component<PhysicsBodyComponent>()->velocity = vec2d(10, 500);
  ent2->get_component<RectColliderComponent>()->on_collision_begin.subscribe(
      [&](Collision &col)
      {
        auto t = ent2->get_component<TransformComponent>();
        auto t2 = col.other->get_component<TransformComponent>();
        g_logger.log_debug("Collision with {} started for main ent. Positions: "
                           "{}, {} and {}, {}",
                           col.other.get_index(), t->position.x, t->position.y,
                           t2->position.x, t2->position.y);
      });
  ent2->get_component<RectColliderComponent>()->on_collision_end.subscribe(
      [&](Collision &col)
      {
        g_logger.log_debug("Collision with {} ended for main ent",
                           col.other.get_index());
      });
  ent2->get_component<PhysicsBodyComponent>()->body_type =
      PhysicsBodyType::DYNAMIC;
  Animation<vec2d> anim;
  // anim.set_target(&ent2->get_component<TransformComponent>()->position);
  anim.add_frame(0, vec2d(0, 0));
  anim.add_frame(
      3, vec2d(root_window->size().x / 2, (int)(root_window->size().y) / 2));
  anim.add_frame(6, vec2d(0, 0));
  anim.add_frame(9, vec2d((int)(root_window->size().x) / -2,
                          (int)(root_window->size().y) / -2));
  anim.add_frame(12, vec2d(0, 0));

  g_logger.log_debug("Starting main loop...");
  while (!root_window->should_close())
  {
    frame++;
    if (time_sys->get_total_millis() > next_fps_time)
    {
      g_logger.log_debug("FPS: {}", frame * 3);
      frame = 0;
      next_fps_time += 333;
    }
    msce::SystemManager::instance->update_all_systems();

    anim.play_frame(time_sys->get_delta_seconds());
    root_window->render();
    root_window->draw();
  }

  return 0;
}