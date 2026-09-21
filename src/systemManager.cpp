#include <MSCE/system.h>
#include <typeindex>
#include <MSCE/BuiltIns/timeSystem.h>

using namespace std;
using namespace msce;

SystemManager::SystemManager() : system_registry_ref_(get_g_system_registry())
{
  logger.log_info("Initializing manager...");

  MSCE_SUBSCRIBE_TO_EVENT_NON_STATIC(RequestDependencyEvent, inject_dependency);

  for (const auto &[type, constructor] :
       this->system_registry_ref_.enumerate_registry())
  {
    this->all_systems.insert(std::pair(type, constructor()));
    this->all_systems.at(type).get()->p_sys_man = this;
    logger.log_debug("Instantiated {}...", Platform::demangle(type.name()));
  }
  logger.log_info("Instantiated {} systems!", this->all_systems.size());

  AllSystemsInstantiatedEvent ev;
  EventManager::instance->fire(ev);
}
void msce::SystemManager::inject_dependency(RequestDependencyEvent &ev) const
{
  try
  {
    ev.system = this->all_systems.at(ev.requested_type).get();
  }
  catch (std::out_of_range e)
  {
    logger.log_error("Can't find system instance of type '{}'",
                     Platform::demangle(ev.requested_type.name()));
  }
}
void SystemManager::init_all_systems()
{
  logger.log_info("Calling pre_init() on all systems...");
  for (const auto &[t, system] : this->all_systems)
  {
    system->pre_init();
  }
  logger.log_info("Calling init() on all systems...");
  for (const auto &[t, system] : this->all_systems)
  {
    system->init();
  }
}

void SystemManager::update_all_systems()
{
  for (auto &[t, system] : this->all_systems)
  {
    if (system->active) system->update(this->time_sys_->get_delta_seconds());
  }
}