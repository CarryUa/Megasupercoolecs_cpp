#include "entity.h"

#include <MSCE/Managers/entityManager.h>

void msce::Entity::set_id(size_t id)
{
    this->entity_id_ = id;
}

size_t msce::Entity::get_id() const
{
    return this->entity_id_;
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