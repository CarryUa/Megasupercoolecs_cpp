#pragma once
#include <MSCE/component.h>
#include <MSCE/Types/color.h>
#include <MSCE/shader.h>

namespace msce
{

struct BaseRendererComponent : public BaseComponent<BaseRendererComponent>
{
private:
  std::string shader_id_ = "DefaultShader";

public:
  Color color = Colors::white;

  virtual std::string &shader_id() noexcept { return shader_id_; }

  MSCE_DEFINE_COMPONENT(BaseRendererComponent, color, shader_id_)
};

} // namespace msce
MSCE_REGISTER_COMPONENT(msce::BaseRendererComponent)