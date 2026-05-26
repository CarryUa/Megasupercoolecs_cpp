#include "systemManager.h"
#include <typeindex>

using namespace std;
using namespace msce;

SystemManager::SystemManager() : system_registry_ref_(get_g_system_registry())
{
    logger.log_info("Initializing self...");
    for (const auto &[type, constructor] : this->system_registry_ref_.enumerate_registry())
    {
        this->all_systems.push_back(constructor());
        this->all_systems.back().get()->p_sys_man = this;
    }

    this->time_sys_ = this->get_system<TimeSystem>();
}
void SystemManager::init_all_systems()
{
    for (const auto &system : this->all_systems)
    {
        system->pre_init();
    }
    for (const auto &system : this->all_systems)
    {
        system->init();
    }
}

void SystemManager::update_all_systems()
{
    for (auto &system : this->all_systems)
    {
        if (system->active)
            system->update(this->time_sys_->get_delta_seconds());
    }
}