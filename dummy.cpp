// This is a dummy file for CMake to determain language correctly when creating
// library. Also used for testing diferent parts of engine
#include <MSCE/msce.h>
#include <MSCE/BuiltIns/Renderers/spriteRendererComponent.hpp>
#include <MSCE/Types/image.hpp>

using namespace msce;

int main(int argc, char **argv)
{

  static auto g_event_man = EventManager();
  static auto g_sys_man = SystemManager();
  static auto g_proto_man = PrototypeManager();
  static auto g_comp_man = ComponentManager();
  static auto g_enum_man = EnumManager();
  static auto g_ent_man = EntityManager();
  // static auto g_shader_man = ShaderManager();

  // ShaderPrototype *shdr =
  //     (ShaderPrototype *)
  //         g_proto_man.create_new_prototype_instance<ShaderPrototype>(
  //             typeof(ShaderPrototype).get_name(), "DefaultShader");

  // shdr->id = "DefaultShader";
  // shdr->fragment_uniforms = {"color"};
  // shdr->fragment_source_path = "testfrag.glsl";
  // shdr->vertex_source_path = "testvert.glsl";
  // g_proto_man.serialize_prototype("test_shader.msceproto_release",
  //                                 "DefaultShader");

  g_sys_man.init_all_systems();
  std::set_terminate(msce::internal::handle_terminate);

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
  while (!root_window->should_close())
  {
    frame++;
    if (time_sys->get_total_millis() > next_fps_time)
    {
      g_logger.log_info("FPS: {}\tTimestamp: {}sec", frame * 3,
                        time_sys->get_total_seconds());
      frame = 0;
      next_fps_time += 333;
    }
    g_sys_man.update_all_systems();

    root_window->render();
    root_window->draw();
  }

  return 0;
}