#include <test_configs.h>
#include <ECS/entity.h>
#include <ECS/component.h>
#include <BuiltInComponents/transformComponent.hpp>
using namespace msce;

TEST(EntityTests, CreationTest)
{
    auto entMan = EntityManager::instance;

    auto ent = entMan->create_entity();
    auto ent2 = entMan->create_entity();

    EXPECT_NE(ent, ent2);
    EXPECT_NE(ent->get_entity_id(), ent2->get_entity_id());
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
        EXPECT_NE(ent->get_entity_id(), ent3->get_entity_id()) << "Copy of an entity has same ID as original";
        ASSERT_TRUE(ent3->has_component<TransformComponent>()) << "Copy of an entity doesn't have same components";
        EXPECT_NE(ent->get_component<TransformComponent>(), ent3->get_component<TransformComponent>()) << "Entity copys component points to same address as originals component";
        EXPECT_EQ(ent->get_component<TransformComponent>()->position, ent3->get_component<TransformComponent>()->position) << "Entity copys component has different values than originals";

        ent->detach_component<TransformComponent>();
        EXPECT_FALSE(ent->has_component<TransformComponent>());
    }
}