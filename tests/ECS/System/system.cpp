#include <gtest/gtest.h>
#include <ECS/Managers/systemManager.h>
#include <ECS/system.h>

using namespace msce;

namespace
{
    class TestSystem1 : public System
    {
    private:
        void SwitchValue()
        {
            value = !value;
        }

    public:
        bool value = false;
        void init() override
        {
            System::init();
            SwitchValue();
        }
    };
}

TEST(SystemTests, SystemCreationCheck)
{
    SystemManager sysMan;

    EXPECT_GE(sysMan.AllSystems.size(), 0);
}

TEST(SystemTests, SystemInheritanceTest)
{
    SystemManager::register_system<TestSystem1>();

    SystemManager sysMan = SystemManager();

    sysMan.init_all_systems();
    TestSystem1 *sys = sysMan.get_system<TestSystem1>();

    // Ensure that sysMan.GetSystem<TestSystem1>() works
    // Assert is to prevent crashes it doesn't
    ASSERT_TRUE(sys != nullptr);

    // Basicaly ensures that sysMan.InitAllSystems(); works, see TestSystem1 for details.
    EXPECT_TRUE(sys->value);

    TestSystem1 *sys_second = sysMan.get_system<TestSystem1>();

    // Ensure that same address is returned every time.
    EXPECT_EQ(sys, sys_second);
}