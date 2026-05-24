#ifndef MSCE_EVENT_MANAGER_H_
#define MSCE_EVENT_MANAGER_H_
#include <MSCE/Types/singleton.hpp>
#include <MSCE/Types/Collections/smartUniquePointerList.hpp>
#include <MSCE/Events/event.h>
#include <unordered_map>
#include <type_traits>
#include <typeindex>
#include <functional>

namespace msce
{

    /**
     *  @brief Manages registration and invocation(firing) of events.
     */
    class EventManager : public Singleton<EventManager>
    {
    private:
        /**
         * @brief Stores individual event subscriptions.
         * @note Essentially a subscription is simply a pair of event type and callback function.
         */
        std::unordered_multimap<std::type_index, std::function<void(BaseEvent &)>>
            subscriptions_;

    public:
        /**
         * Subscribes
         */
        template <typename TEv>
        void subscribe(std::function<void(TEv &)> callback);

        template <typename TEv>
        void fire(TEv &event);
    };

    template <typename TEv>
    inline void EventManager::subscribe(std::function<void(TEv &)> callback)
    {
        static_assert(std::is_base_of_v<BaseEvent, TEv>, "TEv must inherrit from BaseEvent");
        this->subscriptions_.emplace(std::type_index(typeid(TEv)),
                                     [cb = std::move(callback)](BaseEvent &ev)
                                     {
                                         cb(static_cast<TEv &>(ev));
                                     });
    }

    template <typename TEv>
    inline void EventManager::fire(TEv &event)
    {
        static_assert(std::is_base_of_v<BaseEvent, TEv>, "TEv must inherrit from BaseEvent");
        auto range = this->subscriptions_.equal_range(std::type_index(typeid(TEv)));

        for (auto ev = range.first; ev != range.second; ++ev)
        {
            ev->second(static_cast<BaseEvent &>(event));
        }
    }
}

#endif