#include <test_configs.h>
using namespace msce;

namespace
{
    class TestComponent1 : public BaseComponent<TestComponent1>
    {
    public:
        int test_int = 0;
        MSCE_DEFINE_COMPONENT(TestComponent1, test_int)
    };
}
MSCE_REGISTER_COMPONENT(TestComponent1)

TEST(ComponentTests, CreationTest)
{
    ComponentManager *manager = ComponentManager::instance;
    ASSERT_NE(manager, nullptr);
    for (size_t i = 0; i < TEST_ITERATIONS; i++)
    {
        TestComponent1 *comp = manager->create_component<TestComponent1>();
        TestComponent1 *comp2 = manager->create_component<TestComponent1>();

        EXPECT_NE(comp->get_id(), comp2->get_id()) << "2 different components have same ID after creation.";
        EXPECT_NE(comp, comp2) << "2 different components have same address after creation.";

        comp->test_int = rand() % 0xFFFFFFF;
        TestComponent1 *comp_copy = manager->clone_component(comp);
        EXPECT_EQ(comp_copy->test_int, comp->test_int) << "Component copy member values differ from original";
        EXPECT_NE(comp_copy, comp) << "A copy of a component has same address.";
        EXPECT_NE(comp_copy->get_id(), comp->get_id()) << "A copy of a component has same id.";
    }
}

TEST(ComponentTests, ComponentManagerOperationTests)
{
    ComponentManager *manager = ComponentManager::instance;

    for (auto *comp : manager->get_all_components_of_type<IComponent>())
    {
        manager->destroy_component(comp);
    }
    ASSERT_EQ(manager->count(), 0) << "Destroy method doesn't work";

    for (size_t i = 1; i <= TEST_ITERATIONS; i++)
    {

        auto comp1 = manager->create_component<TestComponent1>();
        auto comp2 = manager->create_component<TestComponent1>();

        // Getting all components of one type(or children)
        EXPECT_EQ(manager->count(), 2 * i);

        // Getting one component by id
        EXPECT_EQ(manager->get_component<TestComponent1>(comp1->get_id()), comp1);

        // Clonning component
        auto copy = manager->clone_component<TestComponent1>(comp1); // Mostly to see timestamps.

        // Deleting component
        manager->destroy_component(copy);
        EXPECT_EQ(manager->count(), 2 * i) << "Destroy method doesn't work";
    }
}