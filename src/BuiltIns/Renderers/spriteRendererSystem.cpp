#include <MSCE/BuiltIns/Renderers/spriteRendererSystem.h>

#include <MSCE/BuiltIns/Renderers/spriteRendererComponent.hpp>
#include <MSCE/BuiltIns/entityPrototype.hpp>
#include <MSCE/event.h>

void msce::SpriteRendererSystem::init()
{
  EventManager::instance->subscribe<ObjectBeingRenderedEvent>(
      this->on_object_render);

  EventManager::instance->subscribe<PrototypeLoadedEvent>(
      this->on_prototype_loaded);
}

void msce::SpriteRendererSystem::on_object_render(
    msce::ObjectBeingRenderedEvent &ev)
{
  auto rend = ev.entity_used.get_component<msce::SpriteRendererComponent>();
  if (!rend) return;
  if (!rend->img) return;

  if (!ev.target_window.image_bound(*rend->img))
    ev.target_window.bind_image(*rend->img);

  if (rend->img->format() == ImageFormat::INVALID) return;

  auto shader = ev.target_window.get_shader_ref(rend->shader_id());
  if (!shader->is_ready()) return;

  GLuint texture = 0;
  if (!ev.target_window.try_get_texture(*rend->img, texture)) return;

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  if (shader->unitorm_locations.contains(SHADER_TEXTURE_UNIFORM_NAME))
    glUniform1i(shader->unitorm_locations.contains(SHADER_TEXTURE_UNIFORM_NAME),
                0);
}

void msce::SpriteRendererSystem::on_prototype_loaded(PrototypeLoadedEvent &ev)
{
  static Logger logger("\n\n");
  logger.log_debug("Prototype loaded");
  if (!ev.prototype) return;
  logger.log_debug("Prototype exists");
  if (ev.prototype->get_type_info_polymorphic() != typeof(EntityPrototype))
    return;
  logger.log_debug("Prototype is entproto");
  EntityPrototype *proto = dynamic_cast<EntityPrototype *>(ev.prototype);
  if (!proto) return;
  logger.log_debug("Prototype cast");
  if (!proto->entity) return;
  logger.log_debug("Prototype has entity");
  if (!proto->entity->has_component<SpriteRendererComponent>()) return;

  auto comp = proto->entity->get_component<SpriteRendererComponent>();
  logger.log_debug("\nImage path: {}\n", comp->img_path);
  comp->img = std::make_shared<Image>(comp->img_path);
}
