#include <MSCE/component.h>
#include <iostream>

using namespace msce;

msce::ComponentManager::ComponentManager()
{
  logger.log_info("Initializing manager...");
}

std::vector<std::reference_wrapper<const Type>>
msce::ComponentManager::component_types() const
{
  std::vector<std::reference_wrapper<const Type>> res;
  for (const auto &[t, f] : component_factories())
  {
    res.push_back(t);
  }
  return res;
}

ComponentHandle<IComponent>
msce::ComponentManager::create_component(const msce::Type &type)
{
  auto handle =
      this->components_.insert(component_factories().get_entry(type)());
  return handle;
}

bool msce::ComponentManager::destroy_component(size_t id)
{
  return this->components_.remove(id);
}

std::size_t msce::ComponentManager::count() const noexcept
{
  return this->components_.alive_count();
}
