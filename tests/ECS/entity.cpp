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
        double rand_x = rand() / 2;
        double rand_y = rand() / 2;

        double rand_x2 = rand() / 2;
        double rand_y2 = rand() / 2;

        auto entMan = EntityManager::instance;
        auto compMan = ComponentManager::instance;

        auto ent = entMan->create_entity();
        auto ent2 = entMan->create_entity();

        auto transform = compMan->create_component<TransformComponent>();
        transform->position = Vector2D(rand_x, rand_y);

        auto transform2 = compMan->create_component<TransformComponent>();
        transform2->position = Vector2D(rand_x2, rand_y2);

        ent->attach_component(transform);
        ent2->attach_component(transform2);

        ASSERT_TRUE(ent->has_component<TransformComponent>()) << "Failed to add a component to an entity";
        ASSERT_TRUE(ent2->has_component<TransformComponent>()) << "Failed to add a component to an entity";

        EXPECT_EQ(ent->get_component<TransformComponent>(), transform) << "Component address stored in entity is different from managers";
        EXPECT_EQ(ent2->get_component<TransformComponent>(), transform2) << "Component address stored in entity is different from managers";

        EXPECT_EQ(ent->get_component<TransformComponent>()->position, Vector2D(rand_x, rand_y)) << "Values of component stored in an entity and in manager are different";
        EXPECT_EQ(ent2->get_component<TransformComponent>()->position, Vector2D(rand_x2, rand_y2)) << "Values of component stored in an entity and in manager are different";

        auto ent3 = entMan->copy_entity(ent);

        EXPECT_NE(ent, ent3) << "Entity copy points to same memory address";
        EXPECT_NE(ent.get_index(), ent3.get_index()) << "Copy of an entity has same ID as original";
        ASSERT_TRUE(ent3->has_component<TransformComponent>()) << "Copy of an entity doesn't have same components";
        EXPECT_NE(ent->get_component<TransformComponent>(), ent3->get_component<TransformComponent>()) << "Entity copys component points to same address as originals component";
        EXPECT_EQ(ent->get_component<TransformComponent>()->position, ent3->get_component<TransformComponent>()->position) << "Entity copys component has different values than originals";

        ent->detach_component<TransformComponent>();
        EXPECT_FALSE(ent->has_component<TransformComponent>());
    }
}