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
  std::vector<ComponentHandle<IComponent>> components;

private:
  friend class ::cereal::access;
  template <class Archive> void save(Archive &ar) const
  {
    for (auto &c : components)
    {
    }
    ar(cereal::base_class<::msce::IPrototype>(this),
       ::cereal::make_nvp("components", components));
  }
  template <class Archive> void load(Archive &ar)
  {
    ar(cereal::base_class<::msce::IPrototype>(this),
       ::cereal::make_nvp("components", components));
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
        ::msce::get_reflection_of_type("EntityPrototype");
    return t;
  }
};
} // namespace msce
MSCE_REGISTER_PROTOTYPE(msce::EntityPrototype, EntityPrototype, components)

#endif // MSCE_ENTITY_PROTOTYPE_HPP_
