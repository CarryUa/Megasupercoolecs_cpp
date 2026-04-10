#ifndef _MSCE_TRANSFORM_COMPONENT_H_
#define _MSCE_TRANSFORM_COMPONENT_H_
#include <MSCE/ECS/component.h>
#include <MSCE/Common/vector.h>

namespace msce
{
    class TransformComponent : public BaseComponent<TransformComponent>
    {
    public:
        Vector2D<double> position = Vector2D(0.5, 0.5);
        Vector2D<double> scale = Vector2D(1.0, 1.0);

        MSCE_CEREAL_GENERATE_DERIVED_SERIALIZE_METHODS(IComponent, position, scale);
    };
}
CEREAL_REGISTER_TYPE(msce::TransformComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(msce::IComponent, msce::TransformComponent)

#endif //_MSCE_TRANSFORM_COMPONENT_H_