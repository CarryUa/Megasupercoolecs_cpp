#ifndef MSCE_TEST_CONFIGS_H
#define MSCE_TEST_CONFIGS_H
#include <gtest/gtest.h>
#include <cstddef>

#include <ECS/system.h>
#include <ECS/component.h>
#include <ECS/entity.h>

#include <Managers/systemManager.h>
#include <Managers/componentManager.h>
#include <Managers/entityManager.h>

/// @brief The number of iterations in test using it.
const size_t TEST_ITERATIONS = 10;

/// @brief Precision used for checking floating-point types with some tolerance.
const double TEST_FLOATING_POINT_TOLERANCE = 1e-10;

/// @brief Seed used for srand function.
const unsigned int RAND_FUNCTION_SEED = 0x81049854;

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

#endif // MSCE_TEST_CONFIGS_H
