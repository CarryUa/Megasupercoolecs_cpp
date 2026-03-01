#include "systemManager.h"
#include <typeindex>

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

void SystemManager::InitAllSystems()
{
    for (const auto &system : this->AllSystems)
    {
        system->Init();
    }
}

void SystemManager::UpdateAllSystems()
{
    for (auto &system : this->AllSystems)
    {
        system->Update(0.00016);
    }
}
