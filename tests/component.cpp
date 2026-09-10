#include <gtest/gtest.h>
#include <test_configs.h>

#include <MSCE/ECS/component.h>
#include <MSCE/Managers/componentManager.h>

using namespace msce;

namespace
{
/// @brief Test component with various data types
struct TestComponent : public BaseComponent<TestComponent>
{
  int a = 0;
  float b = 0.0f;
  std::string c = "";
  MSCE_DEFINE_COMPONENT(TestComponent, a, b, c)
};

/// @brief Derived test component
struct TestComponent2 : public TestComponent
{
  int d = 0;
  MSCE_DEFINE_DERIVED_COMPONENT(TestComponent2, TestComponent, d)
};

/// @brief Component for testing container operations
struct StorageTestComponent : public BaseComponent<StorageTestComponent>
{
  std::vector<int> data;
  MSCE_DEFINE_COMPONENT(StorageTestComponent, data)
};

} // namespace

MSCE_REGISTER_COMPONENT(TestComponent)
MSCE_REGISTER_COMPONENT(TestComponent2)
MSCE_REGISTER_COMPONENT(StorageTestComponent)

/// @brief Test basic component creation and retrieval
TEST(ComponentTests, Creation)
{
  auto &compMan = *ComponentManager::instance;

  auto comp = compMan.create_component<TestComponent>();

  ASSERT_TRUE(comp);
  EXPECT_EQ(comp, compMan.get_component<TestComponent>(comp.get_index()));
  EXPECT_GE(compMan.count(), 1);

  auto all_components = compMan.get_all_components_of_type<IComponent>();
  EXPECT_GE(all_components.size(), 1);

  ASSERT_TRUE(compMan.destroy_component(comp));
  EXPECT_FALSE(comp);
}

/// @brief Test derived component creation
TEST(ComponentTests, DerivedComponentCreation)
{
  auto &compMan = *ComponentManager::instance;

  auto comp = compMan.create_component<TestComponent>();
  auto comp2 = compMan.create_component<TestComponent2>();

  ASSERT_TRUE(comp);
  ASSERT_TRUE(comp2);

  // Verify correct storage
  EXPECT_EQ(comp, compMan.get_component<TestComponent>(comp.get_index()));
  EXPECT_EQ(comp2, compMan.get_component<TestComponent2>(comp2.get_index()));

  compMan.destroy_component(comp);
  compMan.destroy_component(comp2);
}

/// @brief Test component data mutation
TEST(ComponentTests, DataMutation)
{
  auto &compMan = *ComponentManager::instance;

  auto comp = compMan.create_component<TestComponent>();
  ASSERT_TRUE(comp);

  comp->a = 42;
  comp->b = 3.14f;
  comp->c = "TestString";

  EXPECT_EQ(comp->a, 42);
  EXPECT_FLOAT_EQ(comp->b, 3.14f);
  EXPECT_EQ(comp->c, "TestString");

  // Retrieve and verify data persists
  auto comp_retrieved = compMan.get_component<TestComponent>(comp.get_index());
  ASSERT_TRUE(comp_retrieved);
  EXPECT_EQ(comp_retrieved->a, 42);
  EXPECT_FLOAT_EQ(comp_retrieved->b, 3.14f);
  EXPECT_EQ(comp_retrieved->c, "TestString");

  compMan.destroy_component(comp);
}

/// @brief Test component cloning
TEST(ComponentTests, Cloning)
{
  auto &compMan = *ComponentManager::instance;

  auto comp = compMan.create_component<TestComponent>();
  ASSERT_TRUE(comp);

  comp->a = 123;
  comp->b = 45.67f;
  comp->c = "Original";

  auto copy = compMan.clone_component(comp);
  ASSERT_TRUE(copy);

  // Verify clone is different instance
  EXPECT_NE(copy.get(), comp.get());
  EXPECT_NE(comp.get_index(), copy.get_index());

  // Verify data is equal
  EXPECT_EQ(comp->a, copy->a);
  EXPECT_FLOAT_EQ(comp->b, copy->b);
  EXPECT_EQ(comp->c, copy->c);

  // Modify original and verify clone is unaffected
  comp->a = 999;
  comp->c = "Modified";
  EXPECT_NE(comp->a, copy->a);
  EXPECT_NE(comp->c, copy->c);
  EXPECT_EQ(copy->a, 123);
  EXPECT_EQ(copy->c, "Original");

  compMan.destroy_component(comp);
  compMan.destroy_component(copy);
}

/// @brief Test multiple component instances
TEST(ComponentTests, MultipleInstances)
{
  auto &compMan = *ComponentManager::instance;

  auto comp1 = compMan.create_component<TestComponent>();
  auto comp2 = compMan.create_component<TestComponent>();
  auto comp3 = compMan.create_component<TestComponent2>();

  ASSERT_TRUE(comp1);
  ASSERT_TRUE(comp2);
  ASSERT_TRUE(comp3);

  // Verify they are different
  EXPECT_NE(comp1.get_index(), comp2.get_index());
  EXPECT_NE(comp1.get_index(), comp3.get_index());
  EXPECT_NE(comp2.get_index(), comp3.get_index());

  // Retrieve all components
  auto all_comps = compMan.get_all_components_of_type<IComponent>();
  EXPECT_GE(all_comps.size(), 3);

  compMan.destroy_component(comp1);
  compMan.destroy_component(comp2);
  compMan.destroy_component(comp3);
}

/// @brief Test component destruction
TEST(ComponentTests, Destruction)
{
  auto &compMan = *ComponentManager::instance;

  auto comp = compMan.create_component<TestComponent>();
  auto index = comp.get_index();
  size_t initial_count = compMan.count();

  ASSERT_TRUE(compMan.destroy_component(comp));
  EXPECT_FALSE(comp);
  EXPECT_EQ(compMan.get_component<TestComponent>(index), nullptr);
  EXPECT_LT(compMan.count(), initial_count);
}

/// @brief Test component with complex data types
TEST(ComponentTests, ComplexDataTypes)
{
  auto &compMan = *ComponentManager::instance;

  auto comp = compMan.create_component<StorageTestComponent>();
  ASSERT_TRUE(comp);

  // Add data to vector
  comp->data.push_back(1);
  comp->data.push_back(2);
  comp->data.push_back(3);

  EXPECT_EQ(comp->data.size(), 3);
  EXPECT_EQ(comp->data[0], 1);
  EXPECT_EQ(comp->data[2], 3);

  // Clone and verify
  auto copy = compMan.clone_component(comp);
  ASSERT_TRUE(copy);
  EXPECT_EQ(copy->data.size(), 3);
  EXPECT_EQ(copy->data[1], 2);

  compMan.destroy_component(comp);
  compMan.destroy_component(copy);
}
