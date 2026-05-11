#include <MSCE/msce_macros.h>

#include <gtest/gtest.h>
#include <MSCE/ECS/system.h>
#include <MSCE/Managers/systemManager.h>

class TestSystem1 : public msce::System
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
MSCE_REGISTER_SYSTEM(TestSystem1, TestSystem1)

using namespace msce;

TEST(SystemTests, SystemCreationCheck)
{
    SystemManager *sysMan = SystemManager::instance;

    EXPECT_GE(sysMan->AllSystems.size(), 0);
}

TEST(SystemTests, SystemInheritanceTest)
{

    SystemManager *sysMan = SystemManager::instance;

    TestSystem1 *sys = sysMan->get_system<TestSystem1>();

    // Ensure that sysMan.GetSystem<TestSystem1>() works
    // Assert is to prevent crashes it doesn't
    ASSERT_TRUE(sys != nullptr) << "SystemManager.get_system returned nullptr on valid system.";

    // Basicaly ensures that sysMan.InitAllSystems(); works, see TestSystem1 for details.
    EXPECT_TRUE(sys->value);

    TestSystem1 *sys_second = sysMan->get_system<TestSystem1>();

    // Ensure that same address is returned every time.
    EXPECT_EQ(sys, sys_second);
}