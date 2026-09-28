#pragma once
#include <MSCE/system.h>
#include <MSCE/Events/graphicsEvents.h>
#include <MSCE/prototype.h>

namespace msce
{
class SpriteRendererSystem : public System
{
  virtual void init() override final;

  static void on_object_render(ObjectBeingRenderedEvent &ev);
  static void on_prototype_loaded(PrototypeLoadedEvent &ev);
};
} // namespace msce
MSCE_REGISTER_SYSTEM(msce::SpriteRendererSystem)