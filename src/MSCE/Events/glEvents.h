#ifndef MSCE_GL_EVENTS_H_
#define MSCE_GL_EVENTS_H_
#include "event.h"
#include <MSCE/Graphics/MSCEWindow.h>

namespace msce
{
    struct PreRenderStartEvent : public BaseEvent
    {
        msce::MSCEWindow &target_window;

        PreRenderStartEvent(MSCEWindow &window) : target_window(window)
        {
        }
    };
    struct RenderStartEvent : public BaseEvent
    {
        msce::MSCEWindow &target_window;

        RenderStartEvent(MSCEWindow &window) : target_window(window)
        {
        }
    };
}

#endif