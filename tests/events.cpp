#include <gtest/gtest.h>
#include <test_configs.h>
#include <MSCE/Managers/eventManager.h>

using namespace msce;

namespace
{
    struct TestEvent : BaseEvent
    {
        uint64_t counter = 0;
    };

    void onEvent(TestEvent &ev)
    {
        ev.counter++;
    }
}

TEST(EventTests, SubscriptionInvocationConsistencyTest)
{
    auto evMan = EventManager::instance;

    for (size_t i = 0; i < TEST_ITERATIONS; i++)
        evMan->subscribe<TestEvent>(onEvent);

    auto event = TestEvent();
    evMan->fire(event);

    EXPECT_EQ(event.counter, TEST_ITERATIONS);
}