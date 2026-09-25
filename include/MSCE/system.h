#pragma once
#include <MSCE/platform.h>
#include <MSCE/Types/Collections/registry.hpp>
#include <MSCE/msce_macros.h>
#include <MSCE/logger.h>
#include <MSCE/Events/systemEvents.h>
#include <MSCE/event.h>
#include <typeindex>
namespace msce
{
class System;

inline Registry<std::type_index, std::function<std::unique_ptr<System>()>> &
get_g_system_registry()
{
  static Registry<std::type_index, std::function<std::unique_ptr<System>()>>
      reg;
  return reg;
}
} // namespace msce

#pragma region Systems Macros
#define MSCE_REGISTER_SYSTEM_IMPL(Type, SysNum)                                \
  namespace msce                                                               \
  {                                                                            \
  template <> class Registration<Type>                                         \
  {                                                                            \
  public:                                                                      \
    [[gnu::used]] Registration()                                               \
    {                                                                          \
      static Logger logger("StaticSystemRegistration");                        \
      if (get_g_system_registry().is_registered(typeid(Type)))                 \
      {                                                                        \
        logger.log_warning("Double registration of {} detected!",              \
                           Platform::demangle(typeid(Type).name()));           \
        return;                                                                \
      }                                                                        \
      get_g_system_registry().register_entry(                                  \
          typeid(Type),                                                        \
          []() -> std::unique_ptr<System>                                      \
          {                                                                    \
            return std::unique_ptr<System>(                                    \
                std::make_unique<Type>().release());                           \
          });                                                                  \
      logger.log_info("Registered system of type: {}",                         \
                      Platform::demangle(typeid(Type).name()));                \
    }                                                                          \
  };                                                                           \
  inline Registration<Type> BOOST_PP_CAT(registered_, SysNum);                 \
  }

#define MSCE_REGISTER_SYSTEM(Type) MSCE_REGISTER_SYSTEM_IMPL(Type, __COUNTER__)

#pragma endregion

namespace msce
{
// Forward declaration to prevent co-dependency
class SystemManager;
class TimeSystem;

class System
{
public:
  /// @brief Determains if system is going to recive update() calls. init() and
  /// pre_init() are recived regardless.
  bool active = true;

  /// @brief Pointer to SystemManager that created this system. Usefull for
  /// getting dependency-systems.
  SystemManager *p_sys_man;

  /// @brief Ran during SystemManager.InitAllSystems before init(). Use it to
  /// get dependencies pointers, but dont expect other systems being
  /// initialized.
  virtual void pre_init();

  /// @brief Ran during SystemManager.InitAllSystems right after pre_init();
  virtual void init();

  /// @brief Ran each frame of the program.
  /// @param delta_t Time since last frame in seconds.
  virtual void update(double delta_seconds);

  virtual ~System() = default;
};

/**
 * @brief Marker class for automatic system dependency injection.
 *
 * @note Can be used safely without segfaults in system members, and any other
 * enviroment where @ref msce::SystemManager exists. @par
 * @note Dependencies are injected by @ref msce::SystemManager right before
 * pre_init() is called.
 */
template <typename TSystem> class SystemDependency
{
  TSystem *system_ = nullptr;

  void on_systems_instantiated(AllSystemsInstantiatedEvent &ev)
  {
    auto request = RequestDependencyEvent(typeid(TSystem));
    EventManager::instance->fire(request);
    system_ = dynamic_cast<TSystem *>(request.system);

    if (!system_)
      throw std::runtime_error(
          std::format("Failed to inject dependency '{}'({}).",
                      Platform::demangle(request.requested_type.name()),
                      Platform::demangle(typeid(TSystem).name())));
  }

public:
  SystemDependency()
  {
    MSCE_SUBSCRIBE_TO_EVENT_NON_STATIC(AllSystemsInstantiatedEvent,
                                       on_systems_instantiated);
  }

  TSystem *operator->() const noexcept { return system_; }
  TSystem &operator*() const noexcept { return *system_; }
  TSystem *get_ptr() const noexcept { return system_; }
};

/// @brief Manages the life-cycle of all registered systems.
/// All systems should be registered before SystemManager is created.
/// Only one instance of SystemManager can exist in a programs, or else
/// constructor will throw std::runtime_error.
class SystemManager : public Singleton<SystemManager>
{
private:
  inline static Logger logger = Logger("SystemManager");
  Registry<std::type_index, std::function<std::unique_ptr<System>()>>
      &system_registry_ref_;
  SystemDependency<TimeSystem> time_sys_;

public:
  /// @brief Creates all previously registered systems.
  /// @exception throws is another instance of SystemManager exists in the
  /// program.
  SystemManager();

  void inject_dependency(RequestDependencyEvent &ev) const;

  /// @brief List of all existing system instances.
  std::unordered_map<std::type_index, std::unique_ptr<System>> all_systems;

  /// @brief Initializes all system using System.Init();
  void init_all_systems();

  /// @brief Updated all systems using System.Update(delta_t);
  void update_all_systems();

  /// @brief Returns system of given type.
  /// @tparam TSys The requested system type.
  /// @return The type of system found in SystemManager::AllSystems or nullptr
  /// on faliure.
  template <typename TSys> TSys *get_system()
  {

    if (!all_systems.contains(typeid(TSys))) return nullptr;

    return dynamic_cast<TSys *>(all_systems.at(typeid(TSys)).get());
  }
};

} // namespace msce
MSCE_REGISTER_SYSTEM(msce::System)
