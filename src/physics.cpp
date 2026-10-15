#include <MSCE/physics.h>
#include <MSCE/entity.h>

#include <MSCE/math.h>
using namespace msce;

void msce::PhysicsSystem::init() {}

void msce::PhysicsSystem::update(double dt)
{
  static Logger logger("");

  auto cc = std::move(this->get_collision_candidates());

  for (auto cand : cc)
    apply_collision(std::move(cand));

  for (auto p : ComponentManager::instance
                    ->get_all_components_of_type<PhysicsBodyComponent>())
  {
    if (p->body_type == PhysicsBodyType::STATIC) continue;

    auto t = p->get_entity()->get_component<TransformComponent>();
    auto c = p->get_entity()->get_component<RectColliderComponent>();
    if (!t | !c) continue;

    p->velocity.y += -9.81 * dt;
    p->velocity.move_towards(vec2d(0, 0), p->linear_drag * dt);

    // check bellow
    if (c->is_colliding())
    {
      for (auto &[eid, col] : c->collisions)
        if (col.direction.y < 0)
        {
          p->velocity.y = 0;
          break;
        }
    }
    t->position += p->velocity * dt;
  }
}
