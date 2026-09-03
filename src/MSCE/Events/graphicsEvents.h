#ifndef MSCE_GL_EVENTS_H_
#define MSCE_GL_EVENTS_H_
#include "event.h"
#include <MSCE/Graphics/MSCEWindow.h>
#include <MSCE/BuiltIns/transformComponent.hpp>
#include <MSCE/ECS/entity.h>

namespace msce
{
struct PreRenderStartEvent : public BaseEvent
{
  msce::MSCEWindow &target_window;

  PreRenderStartEvent(MSCEWindow &window) : target_window(window) {}
};
struct RenderStartEvent : public BaseEvent
{
  msce::MSCEWindow &target_window;

  RenderStartEvent(MSCEWindow &window) : target_window(window) {}
};

/** Fired per-RenderComponent(per child-of @ref msce::BaseRendererComponent) */
struct ObjectBeingRenderedEvent : public BaseEvent
{
  msce::MSCEWindow &target_window;
  TransformComponent &transform_used;
  Entity &entity_used;

  ObjectBeingRenderedEvent(msce::MSCEWindow &window,
                           TransformComponent &transform, Entity &ent)
      : target_window(window), transform_used(transform), entity_used(ent)
  {
  }
};

} // namespace msce

#endif