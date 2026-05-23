#include "entity.h"

#include <MSCE/Managers/entityManager.h>

bool msce::Entity::has_component(ComponentHandle<IComponent> component) noexcept
{
    if (component == nullptr)
        return false;

    for (auto &comp : _components)
    {
        if (comp == component)
            return true;
    }
    return false;
}