#ifndef _MSCE_TRANSFORM_COMPONENT_H_
#define _MSCE_TRANSFORM_COMPONENT_H_
#include <MSCE/component.h>
#include <MSCE/Types/shape.h>
#include <MSCE/Types/vector.h>

namespace msce
{
class TransformComponent : public BaseComponent<TransformComponent>
{
public:
  vec2d position = vec2d(0, 0);
  vec2d scale = vec2d(1.0, 1.0);
  vec2d rotation = vec2d(0, 0);

  Shape2D shape = Rectangle(100, 100);

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

public:
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
MSCE_REGISTER_COMPONENT(msce::TransformComponent)

#endif //_MSCE_TRANSFORM_COMPONENT_H_