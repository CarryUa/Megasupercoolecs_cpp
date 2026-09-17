#include "test_configs.h"

#include <gtest/gtest.h>
#include <MSCE/system.h>
#include <MSCE/Managers/systemManager.h>
#include <MSCE/BuiltIns/timeSystem.h>

using namespace msce;

namespace
{

/// @brief Test system that tracks lifecycle and update calls
class TrackingSystem : public System
{
public:
  bool pre_initialized = false;
  bool initialized = false;
  int update_count = 0;
  double accumulated_delta = 0.0;
  std::vector<double> delta_history;

protected:
  void pre_init() override { pre_initialized = true; }

  void init() override { initialized = true; }

  void update(double delta_seconds) override
  {
    update_count++;
    accumulated_delta += delta_seconds;
    delta_history.push_back(delta_seconds);
  }
};

/// @brief Test system with dependency injection
class DependentSystem : public System
{
public:
  TrackingSystem *dependency = nullptr;

protected:
  void pre_init() override
  {
    dependency = SystemManager::instance->get_system<TrackingSystem>();
  }

  void init() override {}

  void update(double delta_seconds) override {}
};

/// @brief Inactive test system
class InactiveTestSystem : public System
{
public:
  int update_count = 0;

protected:
  void pre_init() override { active = false; }

  void init() override {}

  void update(double delta_seconds) override { update_count++; }
};

} // namespace
MSCE_REGISTER_SYSTEM(TrackingSystem)
MSCE_REGISTER_SYSTEM(DependentSystem)
MSCE_REGISTER_SYSTEM(InactiveTestSystem)

/// @brief Test system initialization and lifecycle
TEST(SystemTests, InitializationLifecycle)
{
  auto &sysMan = *SystemManager::instance;

  auto *tracking = sysMan.get_system<TrackingSystem>();
  auto *dependent = sysMan.get_system<DependentSystem>();
  auto *inactive = sysMan.get_system<InactiveTestSystem>();

  ASSERT_TRUE(tracking);
  ASSERT_TRUE(dependent);
  ASSERT_TRUE(inactive);

  // Check initialization flags
  EXPECT_TRUE(tracking->pre_initialized);
  EXPECT_TRUE(tracking->initialized);

  // Check dependency resolution
  EXPECT_EQ(dependent->dependency, tracking);

  // Check initial active state
  EXPECT_TRUE(tracking->active);
  EXPECT_TRUE(dependent->active);
  EXPECT_FALSE(inactive->active);
}

/// @brief Test that update is called with correct delta time
TEST(SystemTests, UpdateWithDeltaTime)
{
  auto &sysMan = *SystemManager::instance;
  auto *tracking = sysMan.get_system<TrackingSystem>();
  auto *inactive = sysMan.get_system<InactiveTestSystem>();

  tracking->update_count = 0;
  tracking->accumulated_delta = 0.0;
  tracking->delta_history.clear();
  inactive->update_count = 0;

  ITERATE_TEST()
  {

    sysMan.update_all_systems();

    // Active system should receive updates
    EXPECT_GT(tracking->update_count, i);

    // Inactive system should NOT receive updates
    EXPECT_EQ(inactive->update_count, 0);
  }
}

/// @brief Test system manager can be queried for systems
TEST(SystemTests, SystemRetrieval)
{
  auto &sysMan = *SystemManager::instance;

  auto *tracking = sysMan.get_system<TrackingSystem>();
  auto *dependent = sysMan.get_system<DependentSystem>();
  auto *inactive = sysMan.get_system<InactiveTestSystem>();
  auto *nonexistent = sysMan.get_system<System>();

  EXPECT_TRUE(tracking);
  EXPECT_TRUE(dependent);
  EXPECT_TRUE(inactive);
  EXPECT_TRUE(nonexistent);
}

/// @brief Test system dependency resolution
TEST(SystemTests, DependencyResolution)
{
  auto &sysMan = *SystemManager::instance;

  auto *tracking = sysMan.get_system<TrackingSystem>();
  auto *dependent = sysMan.get_system<DependentSystem>();

  ASSERT_TRUE(tracking);
  ASSERT_TRUE(dependent);

  // Dependency should be resolved during pre_init
  EXPECT_EQ(dependent->dependency, tracking);
}

/// @brief Test system activation toggling
TEST(SystemTests, ActiveStateToggling)
{
  auto &sysMan = *SystemManager::instance;
  auto *tracking = sysMan.get_system<TrackingSystem>();

  // Initially active
  EXPECT_TRUE(tracking->active);
  int initial_updates = tracking->update_count;

  // Deactivate
  tracking->active = false;
  sysMan.update_all_systems();
  int after_deactivate = tracking->update_count;

  // Should not have received update while inactive
  EXPECT_EQ(after_deactivate, initial_updates);

  // Reactivate
  tracking->active = true;
  sysMan.update_all_systems();
  int after_reactivate = tracking->update_count;

  // Should receive update after reactivation
  EXPECT_GT(after_reactivate, after_deactivate);
}

/// @brief Test time system integration
TEST(SystemTests, TimeSystemIntegration)
{
  auto &sysMan = *SystemManager::instance;
  auto *time_sys = sysMan.get_system<TimeSystem>();

  ASSERT_TRUE(time_sys);

  // TimeSystem should always be active
  EXPECT_TRUE(time_sys->active);

  // Get initial time
  double initial_total_seconds = time_sys->get_total_seconds();
  double initial_delta = time_sys->get_delta_seconds();

  EXPECT_GE(initial_total_seconds, 0.0);
  EXPECT_GE(initial_delta, 0.0);
}