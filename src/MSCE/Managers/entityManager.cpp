#include "entityManager.h"
#include <iostream>

#include <MSCE/ECS/component.h>
#include <MSCE/Managers/componentManager.h>

msce::EntityHandle msce::EntityManager::create_entity()
{

    auto *entity = new Entity();
    return this->entities_.insert(entity);
}

msce::EntityHandle msce::EntityManager::get_entity(uint32_t id)
{
    return this->entities_[id];
}

msce::EntityHandle msce::EntityManager::copy_entity(EntityHandle other)
{
    auto copy = this->create_entity();

    for (const auto &comp : other->get_components<msce::IComponent>())
    {
        copy->force_attach_component(ComponentManager::instance->clone_component(comp));
    }

    return copy;
}
bool msce::EntityManager::destroy_entity(EntityHandle other)
{
    return this->entities_.remove(other);
}
