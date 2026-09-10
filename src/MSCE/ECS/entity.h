#ifndef MSCE_ENTITY_H_
#define MSCE_ENTITY_H_
#include <vector>
#include <MSCE/ECS/component.h>
#include <MSCE/Managers/componentManager.h>
#include <MSCE/Types/Collections/handle.hpp>

namespace msce
{
template <typename T> class SmartUniquePointerList;
class EntityManager;

/**
 * @brief Class representing an entity in ECS architecture. Containt list of
 * @ref msce::ComponentHandle 's attached to it.
 */
class Entity
{
private:
  friend class EntityManager;

  template <typename TComp>
  using ComponentHandle =
      SmartHandle<SmartUniquePointerList<IComponent>, TComp>;

  /**
   * @brief List of @ref msce::ComponentHandle 's attached to this entity.
   */
  std::unordered_map<std::type_index, ComponentHandle<IComponent>> components_ =
      {};
  size_t entity_id_ = 0;

public:
  /**
   * @tparam TComp type of component to check.
   * @retval true if entity has component of type TComp attached.
   * @retval false if entity does not have component of type TComp attached.
   */
  template <typename TComp> bool has_component() const noexcept;

  /**
   * @param @ref msce::ComponentHandle of component to check.
   * @tparam TComp type of component to be checked.
   * @retval true if entity has this exact component attached.
   * @retval false if entity does not have this exact component attached.
   */
  template <typename TComp>
  bool has_component(ComponentHandle<TComp> component) noexcept;

  /**
   * @param type the std::type_index of component to look for.
   */
  bool has_component(const std::type_index &type) noexcept;

  /**
   * @param @ref msce::ComponentHandle of component to check.
   * @tparam TBaseComp type of component to be checked.
   * @retval true if entity has either TBaseComp or any derived component type
   * attached.
   * @retval false if not.
   */
  template <typename TBaseComp> bool has_component_based_on() noexcept;

  /**
   * @tparam TComp Type of component to get.
   * @retval @ref msce::ComponentHandle of component of type TComp - if said
   * component is attached.
   * @retval @ref nullhandle - if it is not attached.
   */
  template <typename TComp>
  ComponentHandle<TComp> get_component() const noexcept;

  /**
   * @tparam TBaseComp The base component.
   * @retval First @ref msce::ComponentHandle of component of either type
   * TBaseComp or any derivetive of TBaseComp attached to this entity.
   * @retval @ref nullhandle - if not found.
   */
  template <typename TBaseComp>
  ComponentHandle<TBaseComp> get_component_based_on();

  /**
   * @returns std::vector of @ref msce::ComponentHandle 's attached to this
   * entity.
   */
  std::vector<ComponentHandle<IComponent>> get_components() noexcept;

  /**
   * @brief Attaches a component to the entity, if component of same type isn't
   * already attached.
   *
   * @tparam TComp Type of component to be attached. Implicit from @ref
   * msce::ComponentHandle
   * @param comp @ref msce::ComponentHandle of component to be attached.
   *
   * @note This will not do anything if component of type TComp is already
   * attached.
   * @note If you want to overwrite the component, see @ref
   * msce::Entity::force_attach_component()
   */
  template <typename TComp>
  void attach_component(ComponentHandle<TComp> comp) noexcept;
  void attach_component(const std::type_index &type,
                        ComponentHandle<IComponent> comp) noexcept;

  /**
   * @brief Detaches the component of type TComp from this entity, if said
   * component is attached.
   */
  template <typename TComp> void detach_component() noexcept;
  void detach_component(const std::type_index &type_index) noexcept;

  /**
   * @brief Attaches a component to the entity regardless. This will detach and
   * destroy the component of TComp previously stored, and attached new instead.
   * @tparam TComp Type of component to be attached. Implicit from @ref
   * msce::ComponentHandle.
   * @param comp The component to be force-attahed.
   *
   * @warning Previous component of type TComp will be destroyed!
   */
  template <typename TComp>
  void force_attach_component(ComponentHandle<TComp> comp) noexcept;
  void force_attach_component(const std::type_index &type_index,
                              ComponentHandle<IComponent> comp) noexcept;
};
template <typename TComp> inline bool Entity::has_component() const noexcept
{
  return components_.contains(typeid(TComp));
}
template <typename TComp>
inline bool Entity::has_component(ComponentHandle<TComp> component) noexcept
{
  static_assert(std::is_base_of_v<IComponent, TComp>,
                "TComp must inherrit from IComponent");

  if (!has_component<TComp>()) return false;

  return (ComponentHandle<IComponent>)component ==
         components_.at(typeid(TComp));
}
template <typename TBaseComp>
inline bool Entity::has_component_based_on() noexcept
{
  if (has_component<TBaseComp>()) return true;

  for (auto &[t, c] : components_)
  {

    auto *candidate = dynamic_cast<TBaseComp *>(c.get());
    if (candidate) return true;
  }

  return false;
}
template <typename TComp>
inline SmartHandle<SmartUniquePointerList<IComponent>, TComp>
Entity::get_component() const noexcept
{
  if (has_component<TComp>())
    return static_cast<ComponentHandle<TComp>>(components_.at(typeid(TComp)));

  return ComponentHandle<TComp>::create_nullptr();
}

template <typename TBaseComp>
inline ComponentHandle<TBaseComp> Entity::get_component_based_on()
{
  if (has_component<TBaseComp>()) return get_component<TBaseComp>();

  for (auto &[t, c] : components_)
  {
    auto *candidate = dynamic_cast<TBaseComp *>(c.get());
    if (candidate) return static_cast<ComponentHandle<TBaseComp>>(c);
  }

  return ComponentHandle<TBaseComp>::create_nullptr();
}

template <typename TComp>
inline void Entity::attach_component(ComponentHandle<TComp> comp) noexcept
{
  static_assert(std::is_base_of_v<IComponent, TComp>,
                "TComp must inherrit from IComponent");

  attach_component(typeid(TComp),
                   static_cast<ComponentHandle<IComponent>>(comp));
}
template <typename TComp>
inline void Entity::force_attach_component(ComponentHandle<TComp> comp) noexcept
{
  static_assert(std::is_base_of_v<IComponent, TComp>,
                "TComp must inherrit from IComponent");
  force_attach_component(typeid(TComp),
                         static_cast<ComponentHandle<IComponent>>(comp));
}
template <typename TComp> inline void Entity::detach_component() noexcept
{
  detach_component(typeid(TComp));
}
} // namespace msce

#endif // MSCE_ENTITY_H_