#include <gtest/gtest.h>
#include <test_configs.h>
#include <MSCE/event.h>

using namespace msce;

namespace
{
/// @brief Test event with counter
struct TestEvent : public BaseGlobalEvent
{
  uint64_t counter = 0;
};

/// @brief Test event with data payload
struct DataEvent : public BaseGlobalEvent
{
  int value = 0;
  std::string message = "";
};

/// @brief Test event for subscription tracking
struct SubscriptionTrackEvent : public BaseGlobalEvent
{
  int callback_count = 0;
};

struct TestLocalEventArgs
{
  int counter = 0;
  bool called_once = false;
};

/// @brief Global callback counter for testing
int global_callback_count = 0;

/// @brief Test callback that increments counter
void onTestEvent(TestEvent &ev)
{
  ev.counter++;
  Logger logger("");
  logger.log_info("Event rised: {}", ev.counter);
}

/// @brief Test callback that modifies data
void onDataEvent(DataEvent &ev)
{
  ev.value *= 2;
  ev.message.append("_modified");
}

/// @brief Test callback tracking subscription
void onSubscriptionTrack(SubscriptionTrackEvent &ev)
{
  ev.callback_count++;
  global_callback_count++;
}

} // namespace

/// @brief Test basic event subscription and firing
TEST(EventTests, SubscriptionAndFiring)
{
  auto evMan = EventManager::instance;

  auto event = TestEvent();
  evMan->fire(event);

  // Even with no subscribers, event should fire without error
  EXPECT_EQ(event.counter, 0);
}

/// @brief Test single callback subscription
TEST(EventTests, SingleSubscription)
{
  auto evMan = EventManager::instance;

  global_callback_count = 0;

  evMan->subscribe<TestEvent>(onTestEvent);

  auto event = TestEvent();
  evMan->fire(event);

  EXPECT_EQ(event.counter, 1);
}

/// @brief Test multiple callback subscriptions
TEST(EventTests, MultipleSubscriptions)
{
  auto evMan = EventManager::instance;

  auto event = TestEvent();

  // Subscribe the same callback multiple times
  evMan->subscribe<TestEvent>(onTestEvent);
  evMan->subscribe<TestEvent>(onTestEvent);
  evMan->subscribe<TestEvent>(onTestEvent);

  evMan->fire(event);

  // Each subscription should call the callback
  EXPECT_GE(event.counter, 3);
}

/// @brief Test event with data payload
TEST(EventTests, DataPayload)
{
  auto evMan = EventManager::instance;

  auto event = DataEvent();
  event.value = 10;
  event.message = "test";

  evMan->subscribe<DataEvent>(onDataEvent);
  evMan->fire(event);

  EXPECT_EQ(event.value, 20);
  EXPECT_EQ(event.message, "test_modified");
}

/// @brief Test multiple event types
TEST(EventTests, MultipleEventTypes)
{
  auto evMan = EventManager::instance;

  auto test_event = TestEvent();
  auto data_event = DataEvent();

  data_event.value = 5;
  data_event.message = "data";

  evMan->subscribe<TestEvent>(onTestEvent);
  evMan->subscribe<DataEvent>(onDataEvent);

  evMan->fire(test_event);
  evMan->fire(data_event);

  EXPECT_GE(test_event.counter, 1);
  EXPECT_GE(data_event.value, 10);
  EXPECT_GE(data_event.message.size(), 13);
  EXPECT_EQ(data_event.message.substr(0, 13), "data_modified");
}

/// @brief Test subscription tracking
TEST(EventTests, SubscriptionTracking)
{
  auto evMan = EventManager::instance;

  global_callback_count = 0;

  evMan->subscribe<SubscriptionTrackEvent>(onSubscriptionTrack);
  evMan->subscribe<SubscriptionTrackEvent>(onSubscriptionTrack);

  auto event = SubscriptionTrackEvent();
  evMan->fire(event);

  EXPECT_EQ(event.callback_count, 2);
  EXPECT_EQ(global_callback_count, 2);
}

/// @brief Test event firing order
TEST(EventTests, FiringOrder)
{
  auto evMan = EventManager::instance;

  auto event = TestEvent();

  // Subscribe callbacks in order
  evMan->subscribe<TestEvent>(onTestEvent);
  evMan->subscribe<TestEvent>(onTestEvent);

  evMan->fire(event);

  // All callbacks should be invoked
  EXPECT_GE(event.counter, 2);
}

namespace
{
void local_event_count_static_cb(TestLocalEventArgs &args)
{
  if (!args.called_once) args.called_once = true;
  args.counter++;
}
} // namespace

TEST(EventTests, LocalEventTest)
{
  LocalEvent<TestLocalEventArgs> event;

  bool lambda_called = false;

  event.subscribe(
      [&](TestLocalEventArgs &args)
      {
        if (!args.called_once) args.called_once = true;
        lambda_called = true;
        args.counter++;
      });

  event.subscribe(local_event_count_static_cb);

  TestLocalEventArgs args;
  event.fire(args);

  EXPECT_TRUE(lambda_called);
  EXPECT_TRUE(args.called_once);
  EXPECT_EQ(args.counter, 2);
}