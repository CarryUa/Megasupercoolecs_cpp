#pragma once
#include "event.h"
#include <typeindex>

namespace msce
{
class System;

struct AllSystemsInstantiatedEvent : public BaseGlobalEvent
{
};
struct AllSystemsPreInitialized : public BaseGlobalEvent
{
};
struct AllSystemsInitialized : public BaseGlobalEvent
{
};

struct RequestDependencyEvent : public BaseGlobalEvent
{
  const std::type_index &requested_type;
  System *system = nullptr;

  RequestDependencyEvent(const std::type_index &type) : requested_type(type) {}
};

} // namespace msce