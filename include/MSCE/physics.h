#pragma once
#include <MSCE/Types/vector.h>
#include <MSCE/BuiltIns/transformComponent.hpp>
#include <MSCE/entity.h>
#include <MSCE/component.h>
#include <MSCE/system.h>

namespace msce
{
class PhysicsSystem;

struct Collision
{
  EntityHandle other;
  vec2d direction;
};

struct RectColliderComponent : public BaseComponent<RectColliderComponent>
{
public:
  std::unordered_map<uint32_t, Collision> collisions;
  Rectangle box;

  bool is_colliding() const;
  LocalEvent<Collision> on_collision_begin;
  LocalEvent<Collision> on_collision_end;

  MSCE_DEFINE_COMPONENT(RectColliderComponent, box)
};
enum class PhysicsBodyType
{
  STATIC,
  DYNAMIC,
};

struct PhysicsBodyComponent : public BaseComponent<PhysicsBodyComponent>
{
  PhysicsBodyType body_type = PhysicsBodyType::DYNAMIC;
  vec2d velocity = vec2d(0, 0);
  double linear_drag = 1;
  float mass = 1.0;

  MSCE_DEFINE_COMPONENT(PhysicsBodyComponent, velocity, linear_drag, mass,
                        body_type)
};

class PhysicsSystem : public System
{
private:
  struct CollisionData
  {
    EntityHandle e, e2;
    ComponentHandle<TransformComponent> t, t2;
    ComponentHandle<RectColliderComponent> c, c2;
  };
  /** @brief Previous collision candidate count. */
  mutable size_t prev_cc_count_ = 0;

  void init() override;
  void update(double dt) override;

  std::vector<CollisionData> get_collision_candidates() const;
  void apply_collision(CollisionData &&data);
};

} // namespace msce

MSCE_REGISTER_COMPONENT(msce::RectColliderComponent)
MSCE_REGISTER_COMPONENT(msce::PhysicsBodyComponent)
MSCE_REGISTER_SYSTEM(msce::PhysicsSystem)