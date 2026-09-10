#include "entityManager.h"
#include <iostream>

#include <MSCE/ECS/component.h>
#include <MSCE/Managers/componentManager.h>
#include <MSCE/Reflection/reflection.h>

msce::EntityManager::EntityManager()
{
  logger.log_info("Initializing manager...");
}

msce::EntityHandle msce::EntityManager::create_entity()
{

  auto *entity = new Entity();
  return this->entities_.insert(entity);
}

msce::EntityHandle msce::EntityManager::get_entity(uint32_t id)
{
  return this->entities_.get_item(id);
}

void msce::EntityManager::do_for_each_entity(
    std::function<void(EntityHandle &)> callback)
{
  for (auto ent : entities_)
    callback(ent);
}

std::vector<msce::EntityHandle> msce::EntityManager::get_entities()
{
  std::vector<EntityHandle> result(entities_.alive_count(),
                                   EntityHandle::create_nullptr());

  uint32_t i = 0;
  for (const auto &ent : entities_)
  {
    if (!ent) continue;
    result[i++] = ent;
  }

  return result;
}

msce::EntityHandle msce::EntityManager::copy_entity(EntityHandle other)
{
  auto copy = this->create_entity();
  for (auto &[t, comp] : other->components_)
  {
    auto cc = ComponentManager::instance->clone_component(comp);
    copy->force_attach_component(t, cc);
  }

  return copy;
}
bool msce::EntityManager::destroy_entity(EntityHandle other)
{
  for (auto &[_, comp] : other.get()->components_)
    ComponentManager::instance->destroy_component(comp);

  return this->entities_.remove(other);
}
