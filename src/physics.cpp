#include <MSCE/physics.h>
#include <MSCE/entity.h>

#include <MSCE/math.h>

void msce::PhysicsSystem::init() {}

void msce::PhysicsSystem::update(double dt)
{
  static Logger logger("");
  for (auto c : ComponentManager::instance
                    ->get_all_components_of_type<PhysicsBodyComponent>())
  {
    auto t = c->get_entity()->get_component<TransformComponent>();
    if (!t) continue;

    c->velocity.y += -9.81;

    c->velocity.move_towards(vec2d(0, 0), c->linear_drag * dt);
    t->position += c->velocity * dt;

    logger.log_debug("velocity: {}, {}", c->velocity.x, c->velocity.y);
  }
}
