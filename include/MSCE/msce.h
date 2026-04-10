#ifndef MSCE_ENGINE_H_
#define MSCE_ENGINE_H_

#include <MSCE/msce_macros.hpp>
#include <MSCE/Graphics/MSCEWindow.h>

#include <MSCE/ECS/component.h>
#include <MSCE/ECS/entity.h>
#include <MSCE/ECS/system.h>

#include <MSCE/Common/vector.h>
#include <MSCE/Common/Colors/rgba.h>
#include <MSCE/Common/Interfaces/Singleton.hpp>

#include <MSCE/BuiltInComponents/transformComponent.hpp>

#include <MSCE/BuiltInSystems/graphicsSystem.h>
#include <MSCE/BuiltInSystems/timeSystem.h>

#include <MSCE/Managers/componentManager.h>
#include <MSCE/Managers/entityManager.h>
#include <MSCE/Managers/prototypeManager.h>
#include <MSCE/Managers/systemManager.h>

#include <MSCE/Prototypes/prototype.hpp>
#include <MSCE/Prototypes/entityPrototype.hpp>

#endif