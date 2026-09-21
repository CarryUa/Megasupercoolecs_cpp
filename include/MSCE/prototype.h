#pragma once

#include <string>
#include <MSCE/logger.h>
#include <MSCE/reflection.h>
#include <MSCE/Types/Collections/registry.hpp>
#include <MSCE/msce_macros.h>

namespace msce
{
class IPrototype;
inline Registry<std::string, std::reference_wrapper<const std::type_info>> &
get_g_registered_prototypes()
{
  static Registry<std::string, std::reference_wrapper<const std::type_info>>
      reg;
  return reg;
};
inline Registry<std::string, std::function<std::unique_ptr<IPrototype>()>> &
get_g_registered_prototype_factories()
{
  static Registry<std::string, std::function<std::unique_ptr<IPrototype>()>>
      reg;
  return reg;
};
} // namespace msce

#pragma region Prototype Macros

#define MSCE_DEFINE_PROTOTYPE(ProtoType, ...)                                  \
private:                                                                       \
  MSCE_CEREAL_GENERATE_DERIVED_SERIALIZE_METHODS(::msce::IPrototype,           \
                                                 __VA_ARGS__)                  \
public:                                                                        \
  MSCE_REFLECTION_DEFINE_CLASS(ProtoType)                                      \
  virtual const ::msce::Type &get_type_info_polymorphic() override             \
  {                                                                            \
    static const ::msce::Type &t = ::msce::get_reflection_of_type(#ProtoType); \
    return t;                                                                  \
  }

#define MSCE_REGISTER_PROTOTYPE(Type, Name, ...)                               \
  CEREAL_REGISTER_TYPE(Type)                                                   \
  CEREAL_REGISTER_POLYMORPHIC_RELATION(::msce::IPrototype, Type)               \
  MSCE_REFLECT_CLASS(Type, __VA_ARGS__)                                        \
  namespace msce                                                               \
  {                                                                            \
  template <> class Registration<Type>                                         \
  {                                                                            \
  public:                                                                      \
    [[gnu::used]] Registration()                                               \
    {                                                                          \
      static Logger logger("StaticPrototypeRegistration");                     \
      get_g_registered_prototypes().register_entry(#Type,                      \
                                                   std::cref(typeid(Type)));   \
      get_g_registered_prototype_factories().register_entry(                   \
          #Type, []() { return std::make_unique<Type>(); });                   \
      logger.log_info(                                                         \
          "Registered prototype({}) with name: '{}'",                          \
          get_g_registered_prototypes().enumerate_registry().size(), #Type);   \
    }                                                                          \
  };                                                                           \
  inline Registration<Type> registered_##Name;                                 \
  }

#pragma endregion

namespace msce
{
/**
 * @brief Interface and base class of all prototypes.
 */
class IPrototype
{
public:
  /// @brief The if of this prototype.
  std::string id = "Invalid default prototype!";

  /// @brief Do I really need to document this?
  virtual ~IPrototype() = default;

public:
  friend class ::cereal::access;
  /// @brief cereal stuff.
  template <class Archive> void save(Archive &ar) const
  {
    ar(::cereal::make_nvp("id", id));
  }
  /// @brief cereal stuff.
  template <class Archive> void load(Archive &ar)
  {
    ar(::cereal::make_nvp("id", id));
  }
  template <typename T>
  friend constexpr const ::msce::Type &get_reflection_of_type();
  inline static const ::msce::Type &get_type_info()
  {
    static const ::msce::Type &t =
        ::msce::get_reflection_of_type("::msce::IPrototype");
    return t;
  }

  virtual const ::msce::Type &get_type_info_polymorphic()
  {
    static const ::msce::Type &t =
        ::msce::get_reflection_of_type("::msce::IPrototype");
    return t;
  }
};

/**
 * @brief Manages prototypes (de)serialization and manupulation. Always use it
 * to interact with prototypes.
 */
class PrototypeManager : public Singleton<PrototypeManager>
{
private:
  /**
   * @brief Storage for active prototypes.
   */
  std::unordered_map<std::string, std::unique_ptr<IPrototype>> prototypes_;
  inline static Logger logger = Logger("PrototypeManager");

  void load_all_prototypes();

public:
  PrototypeManager();

  /**
   * @brief Reference to staticaly-registered prototype factories.
   */
  Registry<std::string, std::function<std::unique_ptr<IPrototype>()>>
      &registered_factories_ref;
  /**
   * @brief Reference to staticaly-registered prototype types.
   */
  Registry<std::string, std::reference_wrapper<const std::type_info>>
      &registered_prototypes_ref;

  /**
   * @brief Uses cereal to desetialize prototype file into actiall prototype,
   * and stores it in @ref msce::PrototypeManager::prototypes_;
   * @param path Path to serialized file.
   */
  IPrototype *deserialize_prototype(const std::string &path);

  /**
   * @brief Serializes runtime prototype object into a file. TRUNCATES THE FILE.
   * @param path Path to file to serialize into.
   * @param prototype The prototype reference to be serialized.
   * @warning This will truncate the file at path.
   */
  void serialize_prototype(const std::string &path, const std::string &id);

  /**
   * @brief Gets the prototype with given id.
   * @param id The id of prototype to get.
   * @return Pointer to the prototype or nullptr.
   */
  IPrototype *get_prototype(const std::string &id);

  /**
   * @brief Gets the prototype with given id and of type TProto.
   * @param id The if of prototype to get.
   * @return Pointer to the prototype of nullptr.
   */
  template <typename TProto> TProto *get_prototype(const std::string &id);

  /**
   * @return std::unordered_set of pointers to all prototypes.
   */
  std::unordered_set<IPrototype *> enumerate_prototypes();

  /**
   * @tparam TProto Type of prototypes to be returned.
   * @returns std::unordered_set of pointers to prototypes of type TProto.
   */
  template <typename TProto>
  std::unordered_set<TProto *> enumerate_prototypes();

  /**
   * @brief Creates prototype by its name if registered.
   * @param type The prototype typename it was registered with.
   * @param id The id of the prototype.
   * @return Pointer to instantiated prototype or nullptr.
   */
  IPrototype *create_new_prototype_instance(const std::string &type,
                                            const std::string &id) noexcept;

  /// @brief Creates registered prototype by its type-name string at runtime.
  /// @tparam TProto Type that prototype will be cast to.
  /// @param type The type of the prototype, that it was registered with.
  /// @param id Id that will be asigned to the prototype.
  /// @return Pointer to newly-created prototype or nullptr.
  template <typename TProto>
  TProto *create_new_prototype_instance(const std::string &type,
                                        const std::string &new_id) noexcept;

  /**
   * @param id Try and guess -_-
   * @retval true if exists.
   * @retval false if not.
   */
  bool prototype_id_exists(const std::string &id) const noexcept;

  /**
   * @param type Try and guess -_-
   * @retval true if type is registered.
   * @retval false if not.
   */
  bool prototype_type_registered(const std::string &type) const noexcept;

  /**
   * @brief Deletes the instance of prototype with given id.
   * @param id Try and guess -_-
   * @retval true if prototype was deleted.
   * @retval false if not.
   */
  bool delete_prototype(const std::string &id) noexcept;

  /**
   * @brief Deletes the instance of given prototype.
   * @param proto Pointer to the prototype instance to be deleted.
   * @retval true if prototype was deleted.
   * @retval false if not.
   */
  bool delete_prototype(IPrototype *proto) noexcept;
};

template <typename TProto>
inline TProto *PrototypeManager::get_prototype(const std::string &id)
{
  if (!this->prototypes_.contains(id))
  {
    logger.log_warning("[PROTO]: Prototype with id '{}' doesn't exist", id);
    return nullptr;
  }
  TProto *p = dynamic_cast<TProto *>(this->prototypes_[id].get());
  if (p == nullptr)
  {
    logger.log_warning(
        "[PROTO]: Prototype with id '{}' doesn't match type provided: {}", id,
        typeid(TProto).name());
    return nullptr;
  }

  return p;
}

template <typename TProto>
inline std::unordered_set<TProto *> PrototypeManager::enumerate_prototypes()
{
  std::unordered_set<TProto *> result = std::unordered_set<TProto *>();
  for (const auto &[_, p] : this->prototypes_)
  {
    TProto *cast_p = dynamic_cast<TProto *>(p.get());
    if (cast_p == nullptr) continue;

    result.insert(cast_p);
  }

  return result;
};

template <typename TProto>
inline TProto *PrototypeManager::create_new_prototype_instance(
    const std::string &type, const std::string &new_id) noexcept
{
  if (!registered_factories_ref.is_registered(type))
  {
    logger.log_error("Prototype with type '{} ' wasn't ever registered!", type);
    return nullptr;
  }
  if (this->prototypes_.contains(new_id))
  {
    logger.log_error("Prototype with id '{}' already exist!", new_id);
    return nullptr;
  }

  this->prototypes_[new_id] = registered_factories_ref.get_entry(type)();
  this->prototypes_[new_id]->id = new_id;

  auto ptr = dynamic_cast<TProto *>(this->prototypes_[new_id].get());
  if (ptr == nullptr)
  {
    logger.log_error("Prototype with type '{}' cant exist!", type);
    return nullptr;
  }

  return ptr;
}
} // namespace msce

MSCE_REFLECT_CLASS(::msce::IPrototype, id)
CEREAL_REGISTER_TYPE(msce::IPrototype)
