#pragma once
#include <MSCE/ECS/system.h>
#include <MSCE/Events/graphicsEvents.h>

namespace msce
{

class BaseRendererSystem : public System
{

private:
  virtual void init() override;
  static void on_object_render(msce::ObjectBeingRenderedEvent &ev);
};

} // namespace msce
MSCE_REGISTER_SYSTEM(msce::BaseRendererSystem, BaseRendererSystem)