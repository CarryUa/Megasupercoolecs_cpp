#pragma once
#include <MSCE/Types/vector.h>
#include <MSCE/BuiltIns/transformComponent.hpp>
#include <MSCE/component.h>
#include <MSCE/system.h>

namespace msce
{
struct PhysicsBodyComponent : public BaseComponent<PhysicsBodyComponent>
{
  vec2d velocity = vec2d(0, 0);
  double linear_drag = 1;
  float mass = 1.0;

  MSCE_DEFINE_COMPONENT(PhysicsBodyComponent, velocity, linear_drag, mass)
};

class PhysicsSystem : public System
{
private:
  void init() override;
  void update(double dt) override;
};

} // namespace msce

MSCE_REGISTER_COMPONENT(msce::PhysicsBodyComponent)
MSCE_REGISTER_SYSTEM(msce::PhysicsSystem)
