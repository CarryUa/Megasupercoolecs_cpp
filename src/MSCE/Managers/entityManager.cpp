#include "entityManager.h"
#include <iostream>
#include <MSCE/ECS/component.h>
#include <MSCE/Managers/componentManager.h>

msce::Entity *msce::EntityManager::create_entity()
{
    {
        _entities.push_back(std::make_unique<Entity>());
        return _entities.back().get();
    }
}

msce::Entity *msce::EntityManager::get_entity(size_t id)
{
    if (id >= this->_entities.size())
        return nullptr;

    return this->_entities[id].get();
}

size_t msce::EntityManager::get_entity_id(Entity *ent)

{
    size_t i = 0;
    bool found = false;
    for (; i < _entities.size(); ++i)
    {
        if (_entities[i].get() == ent)
        {
            found = true;
            break;
        }
    }

    if (!found)
        std::cerr << "Couldn't find entity with id '" << i << "'" << std::endl;

    return i;
}
msce::Entity *msce::EntityManager::copy_entity(msce::Entity *other)
{
    auto copy = this->create_entity();

    for (auto *comp : other->get_components<msce::IComponent>())
    {
        copy->force_attach_component(comp->clone());
    }

    return copy;
}
void msce::EntityManager::destroy_entity()
{
}