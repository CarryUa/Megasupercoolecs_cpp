#pragma once

#include "event.h"

namespace msce
{
struct PrototypeLoadingStartingEvent : public BaseGlobalEvent
{
};
struct PrototypeLoadingFinishedEvent : public BaseGlobalEvent
{
};
} // namespace msce