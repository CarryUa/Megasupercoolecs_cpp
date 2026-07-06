#pragma once
#include <MSCE/ECS/component.h>
#include <MSCE/Graphics/color.h>
#include <MSCE/Graphics/shaders.h>

namespace msce
{
    struct BaseRendererComponent : public BaseComponent<BaseRendererComponent>
    {
    public:
        Color color = Colors::white;
        std::string shader_id = "DefaultShader";

        MSCE_DEFINE_COMPONENT(BaseRendererComponent, color)
    };
}