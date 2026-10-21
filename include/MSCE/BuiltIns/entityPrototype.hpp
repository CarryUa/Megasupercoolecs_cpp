#ifndef MSCE_ENTITY_PROTOTYPE_HPP_
#define MSCE_ENTITY_PROTOTYPE_HPP_
#include <MSCE/prototype.h>
#include <MSCE/entity.h>

namespace msce
{
/// @brief Prototype for all entities. Think of it as of Unity prefabs maybe?
struct EntityPrototype : public IPrototype
{

  Entity entity;
  // EntityHandle entity = EntityHandle::create_nullptr();

public:
  friend class ::cereal::access;
  void save(SerializationOutArchive &ar) const
  {
    // if (!entity) return;

    ar(cereal::base_class<::msce::IPrototype>(this),
       ::cereal::make_nvp("entity", entity));
  }
  void load(SerializationInArchive &ar)
  {
    ar(cereal::base_class<::msce::IPrototype>(this),
       ::cereal::make_nvp("entity", entity));
  }

public:
  template <typename T>
  friend constexpr const ::msce::Type &get_reflection_of_type();
  inline static const ::msce::Type &get_type_info()
  {
    static const ::msce::Type &t =
        ::msce::get_reflection_of_type(typeid(EntityPrototype));
    return t;
  }
  virtual const ::msce::Type &get_type_info_polymorphic() override
  {
    return EntityPrototype::get_type_info();
  }
};
} // namespace msce
MSCE_REGISTER_PROTOTYPE(msce::EntityPrototype, EntityPrototype, entity)

#endif // MSCE_ENTITY_PROTOTYPE_HPP_
