#pragma once

#include <MSCE/logger.h>
#include <MSCE/Types/singleton.hpp>

#include <functional>
#include <type_traits>
#include <typeindex>
#include <vector>

namespace msce
{
/**
 * @brief Base type for all global events.
 * @note Callbacks subscribed to global events, unlike local ones, are
 * subscribed to type of an event.
 * @note To rise a global event, you should use @ref msce::EventManager
 */
class BaseGlobalEvent
{
};

/**
 * @brief Base type for all local events.
 * @note Callbacks subscribed to local events are subscribed to an instance of
 * an event.
 * @note This means that you subscribe your callback to instance of local event
 * object.
 *
 * @remark Think of it as of C# Action's.
 */

template <typename TArg_I> class LocalEvent
{
public:
  using callback_t = std::function<void(TArg_I &)>;

private:
  std::vector<callback_t> cbs_;

public:
  LocalEvent() {}

  void subscribe(callback_t callback) { cbs_.push_back(callback); }
  void fire(TArg_I &arg1)
  {
    if (cbs_.size() <= 0) return;
    for (auto &cb : cbs_)
      cb(arg1);
  }
};

#define MSCE_SUBSCRIBE_TO_EVENT(Event, Callback)                               \
  EventManager::instance->subscribe<Event>(Callback)

#define MSCE_SUBSCRIBE_TO_EVENT_NON_STATIC(Event, Callback)                    \
  EventManager::instance->subscribe<Event>([this](auto &ev)                    \
                                           { this->Callback(ev); })

/**
 *  @brief Manages registration and invocation(firing) of events.
 */
class EventManager : public Singleton<EventManager>
{
private:
  inline static Logger logger = Logger("EventManager");

  /**
   * @brief Stores individual event subscriptions.
   * @note Essentially a subscription is simply a pair of event type and
   * callback function.
   */
  std::unordered_multimap<std::type_index,
                          std::function<void(BaseGlobalEvent &)>>
      subscriptions_;

public:
  EventManager();

  /**
   * @brief Subsctibes callback to an event of type TEv.
   * @tparam TEv type of event
   * @note Note that regardless of event instance, callback subscribes to event
   * @b type. The event instance is used as args passed into callback parameter.
   */
  template <typename TEv, typename TCallback = std::function<void(TEv &)>>
  void subscribe(TCallback callback);
  /**
   * @brief Fires the event of type TEv, and passes event as parameter for
   * callbacks.
   * @tparam TEv type of event to be fired.
   * @param event TEv instance passed as parameter for callbacks.
   */
  template <typename TEv> void fire(TEv &event);
};

template <typename TEv, typename TCallback>
inline void EventManager::subscribe(TCallback callback)
{
  static_assert(std::is_base_of_v<BaseGlobalEvent, TEv>,
                "TEv must inherrit from BaseGlobalEvent");
  this->subscriptions_.emplace(
      std::type_index(typeid(TEv)),
      [cb = std::move(callback)](BaseGlobalEvent &ev)
      {
        if constexpr (std::is_invocable_v<TCallback, TEv &>)
          cb(static_cast<TEv &>(ev));
        else
          cb();
      });
}

template <typename TEv> inline void EventManager::fire(TEv &event)
{
  static_assert(std::is_base_of_v<BaseGlobalEvent, TEv>,
                "TEv must inherrit from BaseGlobalEvent");
  auto range = this->subscriptions_.equal_range(std::type_index(typeid(TEv)));

  for (auto ev = range.first; ev != range.second; ++ev)
  {
    ev->second(static_cast<BaseGlobalEvent &>(event));
  }
}

} // namespace msce
