#ifndef _MSCE_COMPONENT_H_
#define _MSCE_COMPONENT_H_
#include <iostream>
#include <MSCE/msce_macros.h>
#include <MSCE/reflection.h>
#include <MSCE/Types/Collections/handle.hpp>
#include <MSCE/Types/Collections/smartUniquePointerList.hpp>
#include <MSCE/Types/singleton.hpp>
#include <MSCE/serialization.h>
#include <typeindex>

#pragma region Components Macros
#define MSCE_DEFINE_COMPONENT(Type, ...)                                       \
private:                                                                       \
  friend class ::msce::internal::StaticComponentRegistration<Type>;            \
  friend class ::msce::ComponentManager;                                       \
  MSCE_CEREAL_GENERATE_DERIVED_SERIALIZE_METHODS(::msce::BaseComponent<Type>,  \
                                                 __VA_ARGS__)                  \
public:                                                                        \
  virtual void polymophic_save(SerializationOutArchive &ar) const override     \
  {                                                                            \
    ar(*this);                                                                 \
  }                                                                            \
  virtual void polymophic_load(SerializationInArchive &ar) override            \
  {                                                                            \
    ar(*this);                                                                 \
  }                                                                            \
                                                                               \
  /**  @brief This overload is used by cereal. All components must be          \
   * instantiated using ComponentManager. */                                   \
  Type() {}

#define MSCE_REGISTER_COMPONENT(Type)                                          \
  MSCE_REFLECT_CLASS(Type)                                                     \
  CEREAL_REGISTER_TYPE(Type)                                                   \
  CEREAL_REGISTER_POLYMORPHIC_RELATION(::msce::BaseComponent<Type>, Type)      \
  CEREAL_REGISTER_POLYMORPHIC_RELATION(::msce::IComponent,                     \
                                       ::msce::BaseComponent<Type>)            \
  namespace msce::internal                                                     \
  {                                                                            \
  template <> class StaticComponentRegistration<Type>                          \
  {                                                                            \
  public:                                                                      \
    StaticComponentRegistration()                                              \
    {                                                                          \
      ::msce::ComponentManager::component_factories().register_entry(          \
          typeof(Type), []() { return std::make_unique<Type>(); });            \
    }                                                                          \
  };                                                                           \
  inline static ::msce::internal::StaticComponentRegistration<Type>            \
      BOOST_PP_CAT(registration_, __COUNTER__);                                \
  }

/** @brief Macro for defining components inheriting another component. */
#define MSCE_DEFINE_DERIVED_COMPONENT(Type, Parent, ...)                       \
private:                                                                       \
  friend class ::msce::ComponentManager;                                       \
  friend class ::msce::internal::StaticComponentRegistration<Type>;            \
  MSCE_CEREAL_GENERATE_DERIVED_SERIALIZE_METHODS(Parent, __VA_ARGS__)          \
protected:                                                                     \
  IComponent *clone() override                                                 \
  {                                                                            \
    return new Type(static_cast<const Type &>(*this));                         \
  }                                                                            \
                                                                               \
public:                                                                        \
  virtual void polymophic_save(SerializationOutArchive &ar) const override     \
  {                                                                            \
    ar(*this);                                                                 \
  }                                                                            \
  virtual void polymophic_load(SerializationInArchive &ar) override            \
  {                                                                            \
    ar(*this);                                                                 \
  }                                                                            \
  /**  @brief This overload is used by cereal. All components must be          \
   * instantiated using ComponentManager. */                                   \
  Type() {}

/** @brief Macro for registering components inheriting another component. */
#define MSCE_REGISTER_DERIVED_COMPONENT(Type, Parent)                          \
  MSCE_REFLECT_CLASS(Type)                                                     \
  CEREAL_REGISTER_TYPE(Type)                                                   \
  CEREAL_REGISTER_POLYMORPHIC_RELATION(Parent, Type)                           \
  CEREAL_REGISTER_POLYMORPHIC_RELATION(::msce::IComponent,                     \
                                       ::msce::BaseComponent<Type>)            \
  namespace msce::internal                                                     \
  {                                                                            \
  template <> class StaticComponentRegistration<Type>                          \
  {                                                                            \
  public:                                                                      \
    StaticComponentRegistration()                                              \
    {                                                                          \
      ::msce::ComponentManager::component_factories().register_entry(          \
          typeof(Type), []() { return std::make_unique<Type>(); });            \
    }                                                                          \
  };                                                                           \
  inline static ::msce::internal::StaticComponentRegistration<Type>            \
      BOOST_PP_CAT(registration_, __COUNTER__);                                \
  }

#pragma endregion

namespace msce
{

class Entity;
class ComponentManager;
/**
 * @brief Component interface used for storage.
 */
class IComponent
{
protected:
  friend class ::msce::Entity;
  friend class ::msce::ComponentManager;

  virtual void set_owner(Entity *owner) = 0;

public:
  /// @brief This overload is used by cereal. All components must be
  /// instantiated using ComponentManager
  IComponent() {}
  virtual IComponent *clone() = 0;
  virtual Entity *get_entity() = 0;

  friend class ::cereal::access;
  /**  @brief Cereal's save function. Refer to their docs for more info.*/
  template <class Archive> void save(Archive &ar) const {}
  template <class Archive>
  /**  @brief Cereal's load function. Refer to their docs for more info.*/
  void load(Archive &ar)
  {
  }

  virtual void polymophic_save(SerializationOutArchive &ar) const = 0;
  virtual void polymophic_load(SerializationInArchive &ar) = 0;
};

namespace internal
{
template <typename T> class StaticComponentRegistration;

struct ComponentSerializeProxy
{
  const IComponent *comp;
  void save(SerializationOutArchive &ar) const { comp->polymophic_save(ar); }
};

struct ComponentDeserializeProxy
{
  IComponent *comp;
  void load(SerializationInArchive &ar) { comp->polymophic_load(ar); }
};
} // namespace internal

/**
 * @brief Base type for all components.
 */
template <typename TComp> class BaseComponent : public IComponent
{
private:
  Entity *owner_;

  virtual void set_owner(Entity *owner) final { owner_ = owner; };

protected:
  friend class ::msce::Entity;
  friend class ::msce::ComponentManager;

  /**
   * @returns A pointer to the copy.
   */
  IComponent *clone() override
  {
    return new TComp(static_cast<const TComp &>(*this));
  }
  /// @brief This overload is used by cereal. All components must be
  /// instantiated using ComponentManager.
  BaseComponent() {}

public:
  virtual Entity *get_entity() final { return owner_; };
};

/**
 * @brief This is @ref msce::SmartHandle alias for component manager.
 * @tparam TComp the type of component stored under the handle.
 */
template <typename TComp>
using ComponentHandle = SmartHandle<SmartUniquePointerList<IComponent>, TComp>;

/**
 * @brief Manages the component lifetime. You should always use it for component
 * creation/cloning/deletion.
 */
class ComponentManager : public Singleton<ComponentManager>
{
private:
  template <typename T>
  friend class ::msce::internal::StaticComponentRegistration;

  /// @brief Component storage
  SmartUniquePointerList<IComponent> components_;

  inline static Logger logger = Logger("ComponentManager");

  inline static auto &component_factories()
  {
    static Registry<std::reference_wrapper<const Type>,
                    std::function<std::unique_ptr<IComponent>()>>
        reg;
    return reg;
  };

public:
  ComponentManager();

  /**
   * @brief Creates new component.
   * @tparam TComp Type of component to be created.
   * @return @ref msce::ComponentHandle of newly created component.
   */
  template <typename TComp> ComponentHandle<TComp> create_component();

  /**
   * @brief Creates new component.
   * @param type A @ref msce::Type of component that will be created. Use typeof
   * macro to get it.
   * @return @ref msce::ComponentHandle of newly created component.
   */
  ComponentHandle<IComponent> create_component(const msce::Type &type);

  /**
   * @brief Clones new component.
   * @param other @ref msce::ComponentHandle of component that will be cloned.
   * @tparam TComp Type of component to be cloned.
   * @return @ref msce::ComponentHandle of cloned component.
   */
  template <typename TComp>
  ComponentHandle<TComp> clone_component(ComponentHandle<TComp> other);

  /**
   * @brief Gets the component under given id.
   * @param id The id of the component.
   * @tparam TComp Type of component.
   * @return @ref msce::ComponentHandle of component or nullhandle.
   */
  template <typename TComp>
  ComponentHandle<TComp> get_component(std::size_t id) const;

  /**
   * @brief Enumerates all components of given type.
   * @tparam TComp Type of components to be enumerated.
   * @return std::vector of components.
   * @note Note that it will include any component dynamic-castable to TComp.
   */
  template <typename TComp>
  std::vector<ComponentHandle<TComp>> get_all_components_of_type();

  /**
   * @brief Destroys the component provided.
   * @tparam TComp Type of component to be destroyed. It's implicit from @ref
   * msce::ComponentHandle param, and doesn't matter for destruction.
   * @param comp The @ref msce::ComponentHandle of component to be destroyed.
   * @returns true if component was deleted or didn't exist, false otherwise.
   */
  template <typename TComp> bool destroy_component(ComponentHandle<TComp> comp);

  /**
   * @brief Destroys the component with protided id.
   * @param id The id of component to be destroyed.
   * @returns true if component was deleted or didn't exist, false otherwise.
   */
  bool destroy_component(size_t id);

  /**
   * @return Alive components count.
   */
  std::size_t count() const noexcept;
};

template <typename TComp>
inline ComponentHandle<TComp> ComponentManager::create_component()
{
  static_assert(std::is_base_of_v<IComponent, TComp>,
                "TComp must derive from IComponent");
  return static_cast<ComponentHandle<TComp>>(create_component(typeof(TComp)));
}

template <typename TComp>
inline ComponentHandle<TComp>
ComponentManager::clone_component(ComponentHandle<TComp> other)
{
  IComponent *cloned = other->clone();
  return static_cast<ComponentHandle<TComp>>(this->components_.insert(cloned));
}

template <typename TComp>
inline ComponentHandle<TComp>
ComponentManager::get_component(std::size_t id) const
{
  return static_cast<ComponentHandle<TComp>>(this->components_.get_item(id));
}
template <typename TComp>
inline std::vector<ComponentHandle<TComp>>
ComponentManager::get_all_components_of_type()
{
  std::vector<ComponentHandle<TComp>> return_vec;
  for (auto comp : this->components_)
  {
    TComp *result = dynamic_cast<TComp *>(comp.get());
    if (result != nullptr)
      return_vec.push_back(static_cast<ComponentHandle<TComp>>(comp));
  }

  return return_vec;
}
template <typename TComp>
inline bool ComponentManager::destroy_component(ComponentHandle<TComp> comp)
{
  return this->components_.remove(
      static_cast<ComponentHandle<IComponent>>(comp));
}

} // namespace msce

CEREAL_REGISTER_TYPE(msce::IComponent)

#endif // _MSCE_COMPONENT_H_