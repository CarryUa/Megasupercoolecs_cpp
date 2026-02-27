#include "system_registry.h"
#include <ECS/System/systemManager.h>
#include <ECS/System/system.h>

void register_all()
{
    SystemManager::RegisterSystem<System>();
}