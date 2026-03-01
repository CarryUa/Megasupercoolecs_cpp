#include "systemManager.h"
#include <typeindex>

using namespace std;
using namespace msce;

map<type_index, function<unique_ptr<System>()>> SystemManager::_registered_type_constructor_pairs;

SystemManager::SystemManager()
{
    for (const auto &pair : SystemManager::_registered_type_constructor_pairs)
    {
        this->AllSystems.push_back(pair.second());
    }
}

SystemManager::~SystemManager()
{
    for (const auto &uprt_system : this->AllSystems)
    {
        // Free memory of all unique_pointers
        AllSystems.clear();
    }
}

void SystemManager::init_all_systems()
{
    for (const auto &system : this->AllSystems)
    {
        system->pre_init();
    }
    for (const auto &system : this->AllSystems)
    {
        system->init();
    }
}

void SystemManager::update_all_systems()
{
    for (auto &system : this->AllSystems)
    {
        system->update(0.00016);
    }
}
