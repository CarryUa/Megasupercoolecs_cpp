#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <variant>

#include <MSCE/msce_macros.h>
#include <MSCE/logger.h>
#include <MSCE/Types/Collections/registry.hpp>
#include <MSCE/reflection.h>
#include <MSCE/Types/singleton.hpp>

#define MSCE_REGISTER_ENUM_EVP_HELPER(EnumType, Enumerator)                    \
  evp.emplace(#Enumerator, static_cast<int64_t>(EnumType::Enumerator));
#define MSCE_WRAP_REGISTER_ENUM_EVP(r, data, x)                                \
  MSCE_REGISTER_ENUM_EVP_HELPER(data, x)

#define MSCE_REGISTER_ENUM_IMPL(EnumType, ENUMIDENTIFYER, ...)                 \
  MSCE_CEREAL_GENERATE_ENUM_SERIALIZE_METHODS(EnumType, __VA_ARGS__)           \
  template <> class msce::Registration<EnumType>                               \
  {                                                                            \
  public:                                                                      \
    [[gnu::used]] Registration()                                               \
    {                                                                          \
      static ::msce::Logger logger("StaticEnumRegistration");                  \
      ::msce::get_g_enum_types_vec().push_back(std::cref(typeid(EnumType)));   \
      ::msce::get_g_enum_factories_registry().register_entry(                  \
          #EnumType,                                                           \
          []()                                                                 \
          {                                                                    \
            ::std::unordered_map<std::string, int64_t> evp;                    \
            BOOST_PP_SEQ_FOR_EACH(MSCE_WRAP_REGISTER_ENUM_EVP, EnumType,       \
                                  BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))       \
            return Enum(#EnumType, evp);                                       \
          });                                                                  \
      logger.log_info("Registered enum '{}'", #EnumType);                      \
    }                                                                          \
  };                                                                           \
  inline msce::Registration<EnumType> BOOST_PP_CAT(registered_,                \
                                                   ENUMIDENTIFYER);            \
  MSCE_REFLECT_FUNDAMENTAL(EnumType)
#define MSCE_REGISTER_ENUM(EnumType, ...)                                      \
  MSCE_REGISTER_ENUM_IMPL(EnumType, __COUNTER__, __VA_ARGS__)

namespace msce
{
class EnumManager;
class Enum;

Registry<std::string, std::function<Enum()>> &get_g_enum_factories_registry();

std::vector<std::reference_wrapper<const std::type_info>> &
get_g_enum_types_vec();
/**
 * @brief Class representation of enums. Mainly exists for ABI manipulatuion,
 * you can and should use normal enums for most use cases.
 * @note You can't create this enum by yourself, use @ref msce::EnumManager
 * instead.
 */
class Enum
{
private:
  /// @brief @ref msce::Logger for Enum type.
  inline static Logger logger = Logger("Enum");

  friend class EnumManager;

protected:
  /**
   * @brief This enum's enumerator-value pairs.
   */
  const std::unordered_map<std::string, int64_t> enumerator_value_pairs_;
  /// @brief Name of this enum instance. It is same name that you pass into
  /// MSCE_REGISTER_ENUM() macro.
  std::string enum_name_;
  /// @brief Current enum value.
  int64_t value_;

public:
  Enum() = delete;
  Enum(const Enum &other);

  /**
   * @brief The constructor used by @ref msce::EnumManager. Use it instead of
   * this.
   * @param evp Enumerator-Value pairs.
   */
  Enum(const std::string &enum_name_ref,
       const std::unordered_map<std::string, int64_t> &evp);
  void set(const std::string &enumerator_name) noexcept;
  void set(const int64_t value) noexcept;
  std::string get_name();
  std::string get_enumerator_name();
  template <typename T> T get_value() const noexcept;
};

inline Enum::Enum(const Enum &other)
    : enum_name_(other.enum_name_),
      enumerator_value_pairs_(other.enumerator_value_pairs_), value_(0)
{
}

inline Enum::Enum(const std::string &enum_name_ref,
                  const std::unordered_map<std::string, int64_t> &evp)
    : enum_name_(enum_name_ref), enumerator_value_pairs_(evp), value_(0)
{
}
inline void Enum::set(const std::string &enumerator_name) noexcept
{
  if (!this->enumerator_value_pairs_.contains(enumerator_name))
  {
    logger.log_error("{} has no enumerator named '{}'. Value left unchanged.",
                     enum_name_, enumerator_name);
    return;
  }

  this->value_ = this->enumerator_value_pairs_.at(enumerator_name);
}
inline void Enum::set(const int64_t value) noexcept
{
  for (auto &[n, v] : enumerator_value_pairs_)
  {
    if (v == value)
    {
      this->value_ = value;
      return;
    }
  }

  logger.log_error("{} has no enumerator tied to ({})", enum_name_, value);
}
inline std::string Enum::get_name() { return this->enum_name_; }
inline std::string Enum::get_enumerator_name()
{
  for (auto &[e, v] : this->enumerator_value_pairs_)
    if (v == this->value_) return e;

  return "";
}
template <typename T> inline T Enum::get_value() const noexcept
{
  return static_cast<T>(value_);
}

/**
 * @brief Manages the @ref msce::Enum
 * @note @ref msce::Enum is mostly used by outside programs trough ABI, you can
 * and should use normal enums for most use cases.
 */
class EnumManager : public Singleton<EnumManager>
{
private:
  /// @brief Logger instance for EnumManager.
  inline static Logger logger = Logger("EnumManager");
  /// @brief Reference to global enum factories registry. @ref
  /// msce::get_g_enum_factories_registry().
  Registry<std::string, std::function<Enum()>> &registered_enum_factories_ref_;
  std::vector<std::reference_wrapper<const std::type_info>>
      &registered_enum_types_ref_;

public:
  /// @brief Default constructor.
  EnumManager();

  /**
   * @brief Creates a generic enum type.
   * @param enum_name The actual enum(as in keyword) name.
   * @param value Value enumerator name.
   */
  Enum create_enum(const std::string &enum_name, const std::string &value);
  /**
   * @brief Creates a generic enum type.
   * @param enum_name The actual enum(as in keyword) name.
   * @param value Value enumerator name.
   */
  Enum create_enum(const std::string &enum_name, int64_t value);

  /**
   * @returns true if enum with given name was registered, and false otherwise.
   */
  bool enum_exists(const std::string &enum_name) const;

  /**
   * @returns true if enum with given type_info was registered, and false
   * otherwise.
   */
  bool is_type_enum(const std::type_info &t) const;
};

} // namespace msce
