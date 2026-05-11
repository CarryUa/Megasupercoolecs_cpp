#include "componentManager.h"
#include <iostream>
#include <MSCE/ECS/component.h>

using namespace msce;

msce::ComponentManager::ComponentManager()
{
}

void msce::ComponentManager::destroy_component(IComponent *comp)
{
    this->destroy_component(comp->id_);
}

void msce::ComponentManager::destroy_component(size_t id)
{
    this->components_.remove(id);
}

std::size_t msce::ComponentManager::count() const noexcept
{
    return this->components_.alive_count();
}
