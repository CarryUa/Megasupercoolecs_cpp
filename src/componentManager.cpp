#include <MSCE/component.h>
#include <iostream>

using namespace msce;

msce::ComponentManager::ComponentManager()
{
  logger.log_info("Initializing manager...");
}

ComponentHandle<IComponent>
msce::ComponentManager::create_component(const msce::Type &type)
{
  return this->components_.insert(component_factories().get_entry(type)());
}

bool msce::ComponentManager::destroy_component(size_t id)
{
  return this->components_.remove(id);
}

std::size_t msce::ComponentManager::count() const noexcept
{
  return this->components_.alive_count();
}
