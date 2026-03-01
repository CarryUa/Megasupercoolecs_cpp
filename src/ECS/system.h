#ifndef _MSCE_SYSTEM_H_
#define _MSCE_SYSTEM_H_
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <typeindex>

namespace msce
{
    class System
    {
    public:
        /// @brief Determains if system is going to recive update() calls. init() and pre_init() are recived regardless.
        bool active;

        System();

        /// @brief Ran during SystemManager.InitAllSystems before init(). Use it to get dependencies pointers, but dont expect other systems being initialized.
        virtual void pre_init();

        /// @brief Ran during SystemManager.InitAllSystems right after pre_init();
        virtual void init();

        /// @brief Ran each frame of the program.
        /// @param delta_t Time since last frame in seconds.
        virtual void update(double delta_t);
    };
}

#endif // _MSCE_SYSTEM_H_