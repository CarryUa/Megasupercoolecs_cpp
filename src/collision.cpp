#include <MSCE/physics.h>
#include <cmath>

using namespace msce;

namespace
{
static constexpr const double EXTRA_DISTANCE_FOR_CANDIDATES = 100;

static inline bool aabb_possible(const ComponentHandle<TransformComponent> &t,
                                 const ComponentHandle<TransformComponent> &t2)
{
  return (t->shape.shape.index() == 1 && t2->shape.shape.index() == 1) &&
         (t->rotation == vec2d(0, 0) && t2->rotation == vec2d(0, 0));
}
static inline bool aabb_on_axis(double ap, double ap2, double l, double l2)
{
  if (ap == ap2) return true;
  double x = ap + l / 2, xx = ap - l / 2;
  double xmin = std::min(x, xx), xmax = xmin == x ? xx : x;

  double x2 = ap2 + l2 / 2, xx2 = ap2 - l2 / 2;
  double xmin2 = std::min(x2, xx2), xmax2 = xmin2 == x2 ? xx2 : x2;

  return (xmax >= xmin2) && (xmin <= xmax2);
}

static inline bool aabb_coliding(const vec2d p, const vec2d p2,
                                 const Rectangle &r, const Rectangle &r2)
{
  if (p == p2) return true;

  return aabb_on_axis(p.x, p2.x, r.a(), r2.a()) &&
         aabb_on_axis(p.y, p2.y, r.b(), r2.b());
}

static inline bool sat_coliding(ComponentHandle<TransformComponent> t,
                                ComponentHandle<TransformComponent> t2)
{
  if (t->position == t2->position) return true;

  std::vector<vec2d> ps = std::move(t->get_transformed_verts());
  std::vector<vec2d> ps2 = std::move(t2->get_transformed_verts());
  std::vector<vec2d> edges;
  edges.reserve(ps.size() + 1);

  if (ps.size() < 3) return false;

  for (uint64_t i = 0; i < ps.size(); i++)
  {
    vec2d edge = (i + 1 == ps.size()) ? ps[0] - ps[i] : ps[i + 1] - ps[i];

    vec2d sa = vec2d(edge.y, -edge.x);

    double pp = t->position.dot(sa);

    vec2d minp = *std::min_element(ps.begin(), ps.end(),
                                   [](vec2d a, vec2d b) { return a.x < b.x; }),
          maxp = *std::max_element(ps.begin(), ps.end(),
                                   [](vec2d a, vec2d b) { return a.x < b.x; });
    vec2d minp2 = *std::min_element(ps2.begin(), ps2.end(),
                                    [](vec2d a, vec2d b) { return a.x < b.x; }),
          maxp2 = *std::max_element(ps2.begin(), ps2.end(),
                                    [](vec2d a, vec2d b) { return a.x < b.x; });

    double l = maxp.dot(sa) - minp.dot(sa);
    double l2 = maxp2.dot(sa) - minp2.dot(sa);

    double pp2 = t2->position.dot(sa);
    if (!aabb_on_axis(pp, pp2, l, l2)) return false;
  }

  return true;
}
} // namespace
std::vector<PhysicsSystem::CollisionData>
msce::PhysicsSystem::get_collision_candidates() const
{
  std::vector<CollisionData> result;
  result.reserve(prev_cc_count_);

  auto find_candidates = [&](EntityHandle &ent)
  {
    auto t = ent->get_component<TransformComponent>();
    if (!t) return;
    auto c = ent->get_component_based_on<RectColliderComponent>();
    if (!c) return;

    double d = vec2d(c->box.get_w(), c->box.get_h()).length();
    if (d == 0) return;

    EntityManager::instance->do_for_each_entity(
        [&](EntityHandle &ent2)
        {
          if (ent.get_index() <= ent2.get_index()) return;
          auto t2 = ent2->get_component<TransformComponent>();
          if (!t2) return;

          auto c2 = ent2->get_component_based_on<RectColliderComponent>();
          if (!c2) return;
          double d2 = vec2d(c2->box.get_w(), c2->box.get_h()).length();
          if (d2 == 0) return;

          double rt = (d + d2) / 2;
          double dist = t->position.distance_to(t2->position);

          if (rt <= dist)
          {
            if (dist - rt >= EXTRA_DISTANCE_FOR_CANDIDATES &&
                c->collisions.contains(ent2.get_index()) &&
                c2->collisions.contains(ent.get_index()))
            {
              auto col = Collision{
                  .other = ent2,
                  .direction = (t2->position - t->position).normalized()};
              auto col2 = Collision{
                  .other = ent,
                  .direction = (t->position - t2->position).normalized()};

              c->collisions.erase(ent2.get_index());
              c2->collisions.erase(ent.get_index());
              c->on_collision_end.fire(col);
              c2->on_collision_end.fire(col2);
            }

            return;
          };

          if (t->position == t2->position)
          {
            result.push_back(CollisionData{
                .e = ent, .e2 = ent2, .t = t, .t2 = t2, .c = c, .c2 = c2});
            return;
          }

          result.push_back(CollisionData{
              .e = ent, .e2 = ent2, .t = t, .t2 = t2, .c = c, .c2 = c2});
        });
  };

  EntityManager::instance->do_for_each_entity(find_candidates);
  prev_cc_count_ = result.size();
  return result;
}

void msce::PhysicsSystem::apply_collision(CollisionData &&data)
{
  bool is_colliding = false;

  if (aabb_possible(data.t, data.t2))
    is_colliding = aabb_coliding(data.t->position, data.t2->position,
                                 data.c->box, data.c2->box);
  else
    is_colliding = sat_coliding(data.t, data.t2);

  auto col = Collision{.other = data.e2,
                       .direction =
                           (data.t2->position - data.t->position).normalized()};
  auto col2 = Collision{
      .other = data.e,
      .direction = (data.t->position - data.t2->position).normalized()};

  if (is_colliding)
  {
    bool just_began = !data.c->collisions.contains(data.e2.get_index()) &&
                      !data.c2->collisions.contains(data.e.get_index());

    if (just_began)
    {
      data.c->collisions[data.e2.get_index()] = col;
      data.c2->collisions[data.e.get_index()] = col2;

      data.c->on_collision_begin.fire(col);
      data.c2->on_collision_begin.fire(col2);
    }
  }

  else
  {
    bool just_finished = data.c->collisions.contains(data.e2.get_index()) &&
                         data.c2->collisions.contains(data.e.get_index());

    if (just_finished)
    {
      data.c->collisions.erase(data.e2.get_index());
      data.c2->collisions.erase(data.e.get_index());
      data.c->on_collision_end.fire(col);
      data.c2->on_collision_end.fire(col2);
    }
  }
}

bool msce::RectColliderComponent::is_colliding() const
{
  return collisions.size() > 0;
}
