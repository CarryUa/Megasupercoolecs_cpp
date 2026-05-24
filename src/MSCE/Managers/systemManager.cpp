#include "systemManager.h"
#include <typeindex>

using namespace std;
using namespace msce;

Registry<std::type_index, std::function<std::unique_ptr<System>()>> &msce::SystemManager::get_system_registry()
{
    static auto instance = Registry<std::type_index, std::function<std::unique_ptr<System>()>>();
    return instance;
}

SystemManager::SystemManager()
{
    for (const auto &[type, constructor] : SystemManager::get_system_registry().enumerate_registry())
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