#ifndef MSCE_GL_EVENTS_H_
#define MSCE_GL_EVENTS_H_
#include <MSCE/event.h>
#include <MSCE/MSCEWindow.h>
#include <MSCE/BuiltIns/transformComponent.hpp>
#include <MSCE/entity.h>

namespace msce
{
struct WindowCreatedEvent : public BaseGlobalEvent
{
  MSCEWindow &window;
  WindowCreatedEvent(MSCEWindow &win) : window(win) {}
};

struct PreRenderStartEvent : public BaseGlobalEvent
{
  msce::MSCEWindow &target_window;

  PreRenderStartEvent(MSCEWindow &window) : target_window(window) {}
};
struct RenderStartEvent : public BaseGlobalEvent
{
  msce::MSCEWindow &target_window;

  RenderStartEvent(MSCEWindow &window) : target_window(window) {}
};

/** Fired per-RenderComponent(per child-of @ref msce::BaseRendererComponent) */
struct ObjectBeingRenderedEvent : public BaseGlobalEvent
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