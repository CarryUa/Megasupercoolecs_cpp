#include <test_configs.h>

#include <gtest/gtest.h>
#include <MSCE/ECS/entity.h>
#include <MSCE/ECS/component.h>
#include <MSCE/Prototypes/prototype.hpp>
#include <MSCE/Managers/entityManager.h>
#include <MSCE/Managers/prototypeManager.h>
#include <MSCE/Managers/componentManager.h>
#include <MSCE/BuiltIns/transformComponent.hpp>
#include <MSCE/msce_macros.h>
using namespace msce;

namespace
{
    Logger logger("EntityTests");
}

struct TestPrototype1_ent : public msce::IPrototype
{
    int test_int = 15556;
    bool test_bool = true;
    std::string test_str = "Hello World!";

public:
    MSCE_DEFINE_PROTOTYPE(TestPrototype1_ent, test_int, test_bool, test_str)
};
MSCE_REGISTER_PROTOTYPE(TestPrototype1_ent, TestPrototype1_ent)

TEST(EntityTests, CreationTest)
{
    auto entMan = EntityManager::instance;

    auto ent = entMan->create_entity();
    auto ent2 = entMan->create_entity();

    EXPECT_NE(ent, ent2);
    EXPECT_NE(ent.get_index(), ent2.get_index());
}

TEST(EntityTests, EntityOperationTest)
{
    for (size_t i = 0; i < TEST_ITERATIONS; ++i)
    {
        auto t1 = ComponentManager::instance->create_component<TransformComponent>();
        auto t2 = ComponentManager::instance->create_component<TransformComponent>();
        auto e1 = EntityManager::instance->create_entity();
        auto e2 = EntityManager::instance->create_entity();

        e1->attach_component(t1);
        ASSERT_TRUE(e1->has_component(t1)) << "Component attachment failed";
        ASSERT_EQ(EntityManager::instance->get_entity(e1.get_index()), e1);

        e2->attach_component(t2);
        ASSERT_TRUE(e2->has_component(t2)) << "Component attachment failed";
        ASSERT_EQ(EntityManager::instance->get_entity(e2.get_index()), e2);

        auto e3 = EntityManager::instance->copy_entity(e1);
        ASSERT_EQ(EntityManager::instance->get_entity(e3.get_index()), e3);
        auto t3 = ComponentManager::instance->create_component<TransformComponent>();

        EXPECT_EQ(e1->get_component<TransformComponent>(), t1);
        EXPECT_EQ(e2->get_component<TransformComponent>(), t2);

        e3->force_attach_component(t3);
        EXPECT_FALSE(e3->has_component(t1));
        EXPECT_TRUE(e3->has_component<TransformComponent>());
        EXPECT_NE(e3->get_component<TransformComponent>(), t1);

        EntityManager::instance->destroy_entity(e1);
        EXPECT_EQ(e1.get(), nullptr);
        EXPECT_EQ(t1.get(), nullptr);

        EntityManager::instance->destroy_entity(e2);
        EXPECT_EQ(e2.get(), nullptr);
        EXPECT_EQ(t2.get(), nullptr);

        EntityManager::instance->destroy_entity(e3);
        EXPECT_EQ(e3.get(), nullptr);
        EXPECT_EQ(t3.get(), nullptr);
    }
}