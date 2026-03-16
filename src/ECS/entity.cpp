#include "entity.h"

#include <Managers/entityManager.h>

size_t msce::Entity::get_entity_id()
{
    return EntityManager::instance->get_entity_id(this);
}

bool msce::Entity::has_component(IComponent *component)
{
    for (auto *comp : _components)
    {
        if (comp == component)
            return true;
    }
    return false;
}