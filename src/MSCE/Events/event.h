#ifndef MSCE_EVENT_H_
#define MSCE_EVENT_H_
#include <functional>
#include <type_traits>
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

// template <typename TArg_I, typename TArg_II> class LocalEvent
// {
// private:
//   std::vector<callback_t> cbs_;

// public:
//   using callback_t = std::function<void(TArg_I &, TArg_II &)>;

//   void subscribe(callback_t callback) { cbs_.push_back(callback); }
//   void fire(TArg_I &arg1, TArd_II &arg2)
//   {
//     for (auto &cb : cbs_)
//       cb(arg1, arg2);
//   }
// };
// template <typename TArg_I, typename TArg_II, typename TArg_III> class
// LocalEvent
// {
// private:
//   std::vector<callback_t> cbs_;

// public:
//   using callback_t = std::function<void(TArg_I &, TArg_II &)>;

//   void subscribe(callback_t callback) { cbs_.push_back(callback); }
//   void fire(TArg_I &arg1, TArd_II &arg2, TArg_III &arg3)
//   {
//     for (auto &cb : cbs_)
//       cb(arg1, arg2, arg3);
//   }
// };
} // namespace msce
#endif // MSCE_EVENT_H_
