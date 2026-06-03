#include "componentManager.h"
#include <iostream>
#include <MSCE/ECS/component.h>

using namespace msce;

msce::ComponentManager::ComponentManager()
{
    logger.log_info("Initializing manager...");
}

void msce::ComponentManager::destroy_component(size_t id)
{
    this->components_.remove(id);
}

std::size_t msce::ComponentManager::count() const noexcept
{
    return this->components_.alive_count();
}
