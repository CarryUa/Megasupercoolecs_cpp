#include "entityManager.h"
#include <iostream>

#include <MSCE/ECS/component.h>
#include <MSCE/Managers/componentManager.h>

msce::Entity *msce::EntityManager::create_entity()
{
    {
        auto *entity = new Entity();
        this->entities_.insert(entity);
        return entity;
    }
}

msce::Entity *msce::EntityManager::get_entity(size_t id)
{
    return this->entities_[id];
}

msce::Entity *msce::EntityManager::copy_entity(msce::Entity *other)
{
    auto copy = this->create_entity();

    for (auto *comp : other->get_components<msce::IComponent>())
    {
        copy->force_attach_component(ComponentManager::instance->clone_component(comp));
    }

    return copy;
}
bool msce::EntityManager::destroy_entity(Entity *other)
{
    return this->entities_.remove(other);
}
