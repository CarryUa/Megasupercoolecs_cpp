#include "spriteRendererSystem.h"
#include "spriteRendererComponent.hpp"
#include <MSCE/Managers/eventManager.h>

void msce::SpriteRendererSystem::init()
{
  EventManager::instance->subsctibe_render_event(this->on_object_render);
}

void msce::SpriteRendererSystem::on_object_render(
    msce::ObjectBeingRenderedEvent &ev)
{
  auto rend = ev.entity_used.get_component<msce::SpriteRendererComponent>();
  if (!rend) return;
  // if (!rend->img) return;

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
