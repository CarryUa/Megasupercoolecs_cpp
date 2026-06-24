#ifndef MSCE_ENGINE_H_
#define MSCE_ENGINE_H_

#include <MSCE/msce_macros.h>
#include <MSCE/logger.h>
#include <MSCE/Graphics/MSCEWindow.h>
#include <MSCE/Graphics/color.h>

#include <MSCE/Termination/terminator.h>
#include <MSCE/ABI/abi_macros.h>
#include <MSCE/ABI/core_abi.h>

#include <MSCE/ECS/component.h>
#include <MSCE/ECS/entity.h>
#include <MSCE/ECS/system.h>

#include <MSCE/Reflection/reflection.h>
#include <MSCE/Reflection/reflection_concepts.h>

#include <MSCE/Types/enum.hpp>
#include <MSCE/Types/vector.h>
#include <MSCE/Types/singleton.hpp>
#include <MSCE/Types/Collections/handle.hpp>
#include <MSCE/Types/Collections/registry.hpp>
#include <MSCE/Types/Collections/smartUniquePointerList.hpp>

#include <MSCE/BuiltIns/transformComponent.hpp>

#include <MSCE/BuiltIns/graphicsSystem.h>
#include <MSCE/BuiltIns/timeSystem.h>

#include <MSCE/Managers/componentManager.h>
#include <MSCE/Managers/entityManager.h>
#include <MSCE/Managers/prototypeManager.h>
#include <MSCE/Managers/systemManager.h>
#include <MSCE/Managers/eventManager.h>
#include <MSCE/Managers/enumManager.h>
#include <MSCE/Managers/shaderManager.h>

#include <MSCE/Prototypes/prototype.hpp>
#include <MSCE/Prototypes/entityPrototype.hpp>
#include <MSCE/Prototypes/shaderPrototype.hpp>

#endif