#ifndef _MSCE_TRANSFORM_COMPONENT_H_
#define _MSCE_TRANSFORM_COMPONENT_H_
#include <MSCE/component.h>
#include <MSCE/Types/shape.h>
#include <MSCE/Types/vector.h>

namespace msce
{
class TransformComponent;

struct Transform
{
  Transform() = delete;

  static vec2d rotate_point_deg(const vec2d point, const vec2d center,
                                double angle_deg)
  {
    return rotate_point_rad(point, center, angle_deg * (M_PI / 180));
  }

  static vec2d rotate_point_rad(const vec2d point, const vec2d center,
                                double angle_rad);

  static std::vector<vec2d>
  rotate_vectecies(const ComponentHandle<TransformComponent> &t, double angle);
};

class TransformComponent : public BaseComponent<TransformComponent>
{
public:
  vec2d position = vec2d(0, 0);
  vec2d scale = vec2d(1.0, 1.0);
  vec2d rotation = vec2d(0, 0);

  Shape2D shape;

public:
  friend class ::msce::internal::StaticComponentRegistration<
      TransformComponent>;
  friend class ::msce::ComponentManager;
  friend class ::cereal::access;
  template <class Archive> void save(Archive &ar) const
  {
    IComponent::save(ar);
    ar(::cereal::make_nvp("position", position),
       ::cereal::make_nvp("scale", scale),
       ::cereal::make_nvp("rotation", rotation));

    shape.save(ar);
  }
  template <class Archive> void load(Archive &ar)
  {
    IComponent::load(ar);
    ar(::cereal::make_nvp("position", position),
       ::cereal::make_nvp("scale", scale),
       ::cereal::make_nvp("rotation", rotation));

    shape.load(ar);
  }

  std::vector<vec2d> get_transformed_verts();

public:
  virtual constexpr const msce::Type &polymorphic_type() const override
  {
    return typeof(TransformComponent);
  }
  virtual void polymophic_save(SerializationOutArchive &ar) const override
  {
    this->save(ar);
  }
  virtual void polymophic_load(SerializationInArchive &ar) override
  {
    this->load(ar);
  }
  TransformComponent() {}
};
} // namespace msce
MSCE_REGISTER_COMPONENT(msce::TransformComponent, position, scale, rotation,
                        shape)

#endif //_MSCE_TRANSFORM_COMPONENT_H_