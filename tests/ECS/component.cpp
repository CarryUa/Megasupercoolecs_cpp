#include <test_configs.h>
using namespace msce;

namespace
{
    class TestComponent1 : public BaseComponent<TestComponent1>
    {
    };
}

TEST(ComponentTests, CreationTest)
{

    for (size_t i = 0; i < TEST_ITERATIONS; i++)
    {
        ComponentManager *manager = ComponentManager::instance;

        ASSERT_NE(manager, nullptr);

        TestComponent1 *comp = manager->create_component<TestComponent1>();
        TestComponent1 *comp2 = manager->create_component<TestComponent1>();

        EXPECT_NE(manager->get_component_id(comp), manager->get_component_id(comp2)) << "2 different components have same ID after creation.";
        EXPECT_NE(comp, comp2) << "2 different components have same address after creation.";

        TestComponent1 *comp_copy = manager->clone_component(comp);
        EXPECT_NE(comp_copy, comp) << "A copy of a component has same address.";
        EXPECT_NE(manager->get_component_id(comp_copy), manager->get_component_id(comp)) << "A copy of a component has same id.";
    }
}

TEST(ComponentTests, ComponentManagerOperationTests)
{

    for (size_t i = 0; i < TEST_ITERATIONS; i++)
    {
        ComponentManager *manager = ComponentManager::instance;

        for (auto *comp : manager->get_components<IComponent>())
        {
            manager->destroy_component(comp);
        }

        auto comp1 = manager->create_component<TestComponent1>();
        auto comp2 = manager->create_component<TestComponent1>();

        // Getting all components of one type(or children)
        auto components = manager->get_components<TestComponent1>();
        EXPECT_EQ(components.size(), 2);

        // Check id asignment
        EXPECT_EQ(manager->get_component_id(comp1), 0);
        EXPECT_EQ(manager->get_component_id(comp2), 1);

        // Getting one component by id
        auto comp1_2 = manager->get_component<TestComponent1>(manager->get_component_id(comp1));
        EXPECT_EQ(comp1_2, comp1);
    }
}