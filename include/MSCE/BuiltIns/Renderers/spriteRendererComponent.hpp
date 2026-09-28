#pragma once

#include "baseRendererComponent.hpp"
#include <MSCE/Types/image.h>

namespace msce
{

struct SpriteRendererComponent : public BaseRendererComponent
{
private:
  std::string shader_id_ = "SpriteShader";

public:
  std::string img_path;
  std::shared_ptr<Image> img = nullptr;

  virtual std::string &shader_id() noexcept { return shader_id_; }

private:
  friend class ::msce::ComponentManager;
  friend class ::msce::internal::StaticComponentRegistration<
      SpriteRendererComponent>;
  friend class ::cereal::access;
  template <class Archive> void save(Archive &ar) const
  {
    ar(cereal::base_class<BaseRendererComponent>(this),
       ::cereal::make_nvp("img_path", img_path),
       ::cereal::make_nvp("shader_id_", shader_id_));
  }
  template <class Archive> void load(Archive &ar)
  {
    ar(cereal::base_class<BaseRendererComponent>(this),
       ::cereal::make_nvp("img_path", img_path),
       ::cereal::make_nvp("shader_id_", shader_id_));
  }

protected:
  IComponent *clone() override
  {
    return new SpriteRendererComponent(
        static_cast<const SpriteRendererComponent &>(*this));
  }

public:
  virtual void polymophic_save(SerializationOutArchive &ar) const override
  {
    ar(*this);
  }
  virtual void polymophic_load(SerializationInArchive &ar) override
  {
    ar(*this);
  }
  SpriteRendererComponent() {}
};

} // namespace msce
MSCE_REGISTER_DERIVED_COMPONENT(msce::SpriteRendererComponent,
                                msce::BaseRendererComponent)