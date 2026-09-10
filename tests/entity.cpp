#include "test_configs.h"
#include <gtest/gtest.h>

#include <MSCE/Managers/entityManager.h>

using namespace msce;

namespace
{
/// @brief Test component for entity attachment tests
struct TestComponent : public BaseComponent<TestComponent>
{
  int a = 0;
  int b = 0;
  MSCE_DEFINE_COMPONENT(TestComponent, a, b)
};

/// @brief Derived test component
struct TestComponent2 : public TestComponent
{
  int c = 0;
  MSCE_DEFINE_DERIVED_COMPONENT(TestComponent2, TestComponent, c)
};

/// @brief Component for testing multiple attachments
struct StorageComponent : public BaseComponent<StorageComponent>
{
  std::string name = "";
  MSCE_DEFINE_COMPONENT(StorageComponent, name)
};

} // namespace
MSCE_REGISTER_COMPONENT(TestComponent)
MSCE_REGISTER_DERIVED_COMPONENT(TestComponent2, TestComponent)
MSCE_REGISTER_COMPONENT(StorageComponent)

/// @brief Test entity creation and basic properties
TEST(EntityTests, Creation)
{
  auto &entMan = *EntityManager::instance;

  auto ent = entMan.create_entity();

  ASSERT_TRUE(ent);
  EXPECT_EQ(ent.get_index(), ent.get_index());

  auto ent_retrieved = entMan.get_entity(ent.get_index());
  EXPECT_EQ(ent, ent_retrieved);

  entMan.destroy_entity(ent);
  EXPECT_FALSE(ent);
}

/// @brief Test entity component attachment
TEST(EntityTests, ComponentAttachment)
{
  auto &entMan = *EntityManager::instance;
  auto &compMan = *ComponentManager::instance;

  auto ent = entMan.create_entity();
  auto comp = compMan.create_component<TestComponent>();

  ASSERT_TRUE(ent);
  ASSERT_TRUE(comp);

  // Initially no components
  EXPECT_FALSE(ent->has_component<TestComponent>());
  EXPECT_FALSE(ent->has_component(comp));
  EXPECT_TRUE(ent->get_components().empty());

  // Attach component
  ent->attach_component(comp);

  EXPECT_TRUE(ent->has_component<TestComponent>());
  EXPECT_TRUE(ent->has_component(comp));
  EXPECT_EQ(ent->get_component<TestComponent>(), comp);

  entMan.destroy_entity(ent);
  compMan.destroy_component(comp);
}

/// @brief Test entity component detachment
TEST(EntityTests, ComponentDetachment)
{
  auto &entMan = *EntityManager::instance;
  auto &compMan = *ComponentManager::instance;

  auto ent = entMan.create_entity();
  auto comp = compMan.create_component<TestComponent>();

  ASSERT_TRUE(ent);
  ASSERT_TRUE(comp);

  ent->attach_component(comp);
  EXPECT_TRUE(ent->has_component<TestComponent>());

  ent->detach_component<TestComponent>();

  EXPECT_FALSE(ent->has_component<TestComponent>());

  entMan.destroy_entity(ent);
  compMan.destroy_component(comp);
}

/// @brief Test entity with multiple components
TEST(EntityTests, MultipleComponents)
{
  auto &entMan = *EntityManager::instance;
  auto &compMan = *ComponentManager::instance;

  auto ent = entMan.create_entity();
  auto comp1 = compMan.create_component<TestComponent>();
  auto comp2 = compMan.create_component<TestComponent2>();
  auto comp3 = compMan.create_component<StorageComponent>();

  ASSERT_TRUE(ent);
  ASSERT_TRUE(comp1);
  ASSERT_TRUE(comp2);
  ASSERT_TRUE(comp3);

  ent->attach_component(comp1);
  ent->attach_component(comp2);
  ent->attach_component(comp3);

  EXPECT_TRUE(ent->has_component<TestComponent>());
  EXPECT_TRUE(ent->has_component<TestComponent2>());
  EXPECT_TRUE(ent->has_component<StorageComponent>());
  EXPECT_EQ(ent->get_components().size(), 3);

  ent->detach_component<TestComponent>();

  EXPECT_FALSE(ent->has_component<TestComponent>());
  EXPECT_TRUE(ent->has_component<TestComponent2>());
  EXPECT_TRUE(ent->has_component<StorageComponent>());
  EXPECT_EQ(ent->get_components().size(), 2);

  entMan.destroy_entity(ent);
  compMan.destroy_component(comp1);
  compMan.destroy_component(comp2);
  compMan.destroy_component(comp3);
}

/// @brief Test derived component attachment and base type queries
TEST(EntityTests, DerivedComponentQueries)
{
  auto &entMan = *EntityManager::instance;
  auto &compMan = *ComponentManager::instance;

  auto ent = entMan.create_entity();
  auto comp_base = compMan.create_component<TestComponent>();
  auto comp_derived = compMan.create_component<TestComponent2>();

  ASSERT_TRUE(ent);
  ASSERT_TRUE(comp_base);
  ASSERT_TRUE(comp_derived);

  ent->attach_component(comp_derived);

  // Derived component satisfies base type queries
  EXPECT_FALSE(ent->has_component<TestComponent>());
  EXPECT_TRUE(ent->has_component<TestComponent2>());

  EXPECT_TRUE(ent->has_component_based_on<TestComponent>());
  EXPECT_EQ(ent->get_component_based_on<TestComponent>().get_index(),
            comp_derived.get_index());

  entMan.destroy_entity(ent);
  compMan.destroy_component(comp_base);
  compMan.destroy_component(comp_derived);
}

/// @brief Test entity destruction
TEST(EntityTests, Destruction)
{
  auto &entMan = *EntityManager::instance;

  auto ent = entMan.create_entity();
  auto index = ent.get_index();
  size_t initial_count = entMan.get_entities().size();

  ASSERT_TRUE(entMan.destroy_entity(ent));
  EXPECT_FALSE(ent);
  EXPECT_LT(entMan.get_entities().size(), initial_count);
  EXPECT_EQ(entMan.get_entity(index), nullptr);
}

/// @brief Test multiple entities
TEST(EntityTests, MultipleEntities)
{
  auto &entMan = *EntityManager::instance;
  auto &compMan = *ComponentManager::instance;

  auto ent1 = entMan.create_entity();
  auto ent2 = entMan.create_entity();
  auto ent3 = entMan.create_entity();

  ASSERT_TRUE(ent1);
  ASSERT_TRUE(ent2);
  ASSERT_TRUE(ent3);

  // All entities should be distinct
  EXPECT_NE(ent1.get_index(), ent2.get_index());
  EXPECT_NE(ent2.get_index(), ent3.get_index());
  EXPECT_NE(ent1.get_index(), ent3.get_index());

  auto comp1 = compMan.create_component<TestComponent>();
  auto comp2 = compMan.create_component<StorageComponent>();

  ent1->attach_component(comp1);
  ent2->attach_component(comp2);

  EXPECT_TRUE(ent1->has_component<TestComponent>());
  EXPECT_FALSE(ent1->has_component<StorageComponent>());
  EXPECT_FALSE(ent2->has_component<TestComponent>());
  EXPECT_TRUE(ent2->has_component<StorageComponent>());

  entMan.destroy_entity(ent1);
  entMan.destroy_entity(ent2);
  entMan.destroy_entity(ent3);
  compMan.destroy_component(comp1);
  compMan.destroy_component(comp2);
}

/// @brief Test entity component type queries
TEST(EntityTests, ComponentTypeQueries)
{
  auto &entMan = *EntityManager::instance;
  auto &compMan = *ComponentManager::instance;

  auto ent = entMan.create_entity();
  auto comp = compMan.create_component<StorageComponent>();

  ASSERT_TRUE(ent);
  ASSERT_TRUE(comp);

  comp->name = "TestEntity";
  ent->attach_component(comp);

  // Test retrieval by type
  auto retrieved = ent->get_component<StorageComponent>();
  ASSERT_TRUE(retrieved);
  EXPECT_EQ(retrieved->name, "TestEntity");

  // Test existence check
  EXPECT_TRUE(ent->has_component<StorageComponent>());
  EXPECT_TRUE(ent->has_component(comp));

  // Test component list
  auto all = ent->get_components();
  EXPECT_EQ(all.size(), 1);

  entMan.destroy_entity(ent);
  compMan.destroy_component(comp);
}