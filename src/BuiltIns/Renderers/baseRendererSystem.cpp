#include <MSCE/BuiltIns/Renderers/baseRendererSystem.h>

#include <MSCE/BuiltIns/Renderers/baseRendererComponent.hpp>
#include <MSCE/event.h>
#include <MSCE/logger.h>

void msce::BaseRendererSystem::init()
{
  EventManager::instance->subscribe<ObjectBeingRenderedEvent>(
      this->on_object_render);
}

void msce::BaseRendererSystem::on_object_render(
    msce::ObjectBeingRenderedEvent &ev)
{
  auto rend = ev.entity_used.get_component<msce::BaseRendererComponent>();
  if (!rend) return;

  auto shader = ev.target_window.get_shader_ref(rend->shader_id());
  if (!shader->is_ready()) return;

  glUniform4f(shader->unitorm_locations.at(SHADER_COLOR_UNIFORM_NAME),
              (float)rend->color.r(), (float)rend->color.g(),
              (float)rend->color.b(), (float)rend->color.a());
}
