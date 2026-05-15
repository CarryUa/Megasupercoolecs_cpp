#ifndef _MSCE_TRANSFORM_COMPONENT_H_
#define _MSCE_TRANSFORM_COMPONENT_H_
#include <MSCE/ECS/component.h>
#include <MSCE/Graphics/shape.h>
#include <MSCE/Common/vector.h>

namespace msce
{
    class TransformComponent : public BaseComponent<TransformComponent>
    {
    public:
        vec2d position = vec2d(-1, 0);
        vec2d scale = vec2d(1.0, 1.0);
        vec2d rotation = vec2d(0, 0);

        std::shared_ptr<BaseShape2D>
            shape = std::make_shared<Rectangle>();

        MSCE_DEFINE_COMPONENT(TransformComponent, position, scale, shape)
    };
}
MSCE_REGISTER_COMPONENT(msce::TransformComponent)

#endif //_MSCE_TRANSFORM_COMPONENT_H_