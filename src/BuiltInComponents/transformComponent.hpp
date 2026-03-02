#ifndef _MSCE_TRANSFORM_COMPONENT_H_
#define _MSCE_TRANSFORM_COMPONENT_H_
#include <ECS/component.h>
#include <Common/vector.h>

namespace msce
{
    class TransformComponent : public Component
    {
    public:
        using Component::Component;
        Vector2D<double> position = Vector2D(0.5, 0.5);
        Vector2D<double> scale = Vector2D(1.0, 1.0);
    };
}

#endif //_MSCE_TRANSFORM_COMPONENT_H_