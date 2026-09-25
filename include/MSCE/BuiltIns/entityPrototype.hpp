#ifndef MSCE_ENTITY_PROTOTYPE_HPP_
#define MSCE_ENTITY_PROTOTYPE_HPP_
#include <MSCE/prototype.h>
#include <MSCE/entity.h>

namespace msce
{
/// @brief Prototype for all entities. Think of it as of Unity prefabs maybe?
struct EntityPrototype : public IPrototype
{
  /// @brief List of components for serialization.
  EntityHandle entity = EntityHandle::create_nullptr();

public:
  friend class ::cereal::access;
  void save(SerializationOutArchive &ar) const
  {
    if (!entity) return;

    ar(cereal::base_class<::msce::IPrototype>(this),
       ::cereal::make_nvp("entity", *entity.get()));
  }
  void load(SerializationInArchive &ar)
  {
    if (entity != nullptr) EntityManager::instance->destroy_entity(entity);
    entity = EntityManager::instance->create_entity();

    ar(cereal::base_class<::msce::IPrototype>(this),
       ::cereal::make_nvp("entity", *entity.get()));
  }

public:
  template <typename T>
  friend constexpr const ::msce::Type &get_reflection_of_type();
  inline static const ::msce::Type &get_type_info()
  {
    static const ::msce::Type &t =
        ::msce::get_reflection_of_type("EntityPrototype");
    return t;
  }
  virtual const ::msce::Type &get_type_info_polymorphic() override
  {
    static const ::msce::Type &t =
        ::msce::get_reflection_of_type(typeid(EntityPrototype));
    return t;
  }
};
} // namespace msce
MSCE_REGISTER_PROTOTYPE(msce::EntityPrototype, EntityPrototype)

#endif // MSCE_ENTITY_PROTOTYPE_HPP_
