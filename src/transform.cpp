#include <MSCE/BuiltIns/transformComponent.hpp>

msce::vec2d msce::Transform::rotate_point_rad(const vec2d point,
                                              const vec2d center,
                                              double angle_rad)
{
  vec2d d = center - point;
  double s = std::sin(angle_rad), c = std::cos(angle_rad);

  return vec2d{center.x + d.x * c - d.y * s, center.y + d.x * s + d.y * c};
}

std::vector<msce::vec2d>
msce::Transform::rotate_vectecies(const ComponentHandle<TransformComponent> &t,
                                  double angle)
{
  std::vector<vec2d> verts = std::move(t->shape.get_vertecies());
  uint32_t vc = t->shape.get_mesh_ref().vertex_count();

  std::vector<vec2d> result;
  result.reserve(vc);
  for (uint64_t i = 0; i < vc; ++i)
  {
    vec2d point =
        (verts[i] - vec2d(t->shape.get_w(), t->shape.get_h()) / 2) * t->scale +
        t->position;
    result.push_back(rotate_point_deg(point, t->position, angle));
  }

  return result;
}

std::vector<msce::vec2d> msce::TransformComponent::get_transformed_verts()
{
  std::vector<vec2d> res = std::move(this->shape.get_vertecies());

  for (uint64_t i = 0; i < res.size(); ++i)
  {
    res[i] += this->position;
    res[i] -= vec2d(this->shape.get_w(), this->shape.get_h()) / 2;
    res[i] *= this->scale;
    res[i] =
        Transform::rotate_point_deg(res[i], this->position, this->rotation.x);
  }

  return res;
}
