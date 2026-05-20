#ifndef MSCE_TEST_CONFIGS_H
#define MSCE_TEST_CONFIGS_H

/// @brief The number of iterations in test using it.
#define TEST_ITERATIONS 10000

/// @brief Precision used for checking floating-point types with some tolerance.
#define TEST_FLOATING_POINT_TOLERANCE 1.0E-14

/// @brief Precision used for checking floating-point types with some tolerance specificali in trigonometry enviroments.
#define TEST_FLOATING_POINT_TRIGONOMETRY_TOLERANCE 1.0E-9

/// @brief Seed used for srand function.
#define RAND_FUNCTION_SEED 0x81049854

#endif // MSCE_TEST_CONFIGS_H
