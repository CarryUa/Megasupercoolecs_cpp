#include "entity.h"

bool msce::Entity::has_component(const std::type_index &type) noexcept
{
  return components_.contains(type);
}

std::vector<msce::ComponentHandle<msce::IComponent>>
msce::Entity::get_components() noexcept
{
  std::vector<ComponentHandle<IComponent>> result(
      components_.size(), ComponentHandle<IComponent>::create_nullptr());

  for (decltype(components_)::iterator it = components_.begin();
       it != components_.end(); it++)
    result.push_back(it->second);

  return result;
}

void msce::Entity::attach_component(const std::type_index &type,
                                    ComponentHandle<IComponent> comp) noexcept
{
  if (!has_component(type))
  {
    comp->set_owner(this);
    components_.emplace(type, comp);
  }
}

void msce::Entity::detach_component(const std::type_index &type) noexcept
{
  if (has_component(type))
  {
    components_.at(type)->set_owner(nullptr);
    components_.erase(type);
  }
}

void msce::Entity::force_attach_component(
    const std::type_index &type, ComponentHandle<IComponent> comp) noexcept
{
  if (has_component(type)) detach_component(type);

  attach_component(type, comp);
}
