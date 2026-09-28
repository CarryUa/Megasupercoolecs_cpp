#pragma once

#include <MSCE/event.h>

namespace msce
{
struct PrototypeLoadedEvent : public BaseGlobalEvent
{
  IPrototype *prototype;
};

struct AllPrototypesLoadedEvent : public BaseGlobalEvent
{
};
} // namespace msce