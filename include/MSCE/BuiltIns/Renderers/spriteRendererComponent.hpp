#pragma once

#include "baseRendererComponent.hpp"
#include <MSCE/Types/image.h>

namespace msce
{

struct SpriteRendererComponent : public BaseRendererComponent
{
private:
  std::string shader_id_ = "SpriteShader";
  MSCE_REFLECTION_ALLOW_PRIVATE(SpriteRendererComponent)

public:
  std::string img_path;
  std::shared_ptr<Image> img = nullptr;

  virtual std::string &shader_id() noexcept { return shader_id_; }
  MSCE_DEFINE_DERIVED_COMPONENT(SpriteRendererComponent, BaseRendererComponent,
                                img_path, shader_id_)
};

} // namespace msce
MSCE_REGISTER_DERIVED_COMPONENT(msce::SpriteRendererComponent,
                                msce::BaseRendererComponent, img_path,
                                shader_id_)