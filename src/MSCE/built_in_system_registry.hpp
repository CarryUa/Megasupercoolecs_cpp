#ifndef _MSCE_BUILT_IN_SYSTEM_REGISTRY_H_
#define _MSCE_BUILT_IN_SYSTEM_REGISTRY_H_
#include <MSCE/ECS/system.h>
#include <MSCE/BuiltInSystems/graphicsSystem.h>
#include <MSCE/Managers/systemManager.h>

using namespace msce;

void register_built_in_systems()
{
    SystemManager::register_system<System>();
    SystemManager::register_system<GraphicsSystem>();
    SystemManager::register_system<TimeSystem>();
}

#endif
