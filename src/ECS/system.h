#ifndef _MSCE_SYSTEM_H_
#define _MSCE_SYSTEM_H_
namespace msce
{
    // Forward declaration to prevent co-dependency
    class SystemManager;

    class System
    {
    public:
        /// @brief Determains if system is going to recive update() calls. init() and pre_init() are recived regardless.
        bool active = true;

        /// @brief Pointer to SystemManager that created this system. Usefull for getting dependency-systems.
        SystemManager *p_sys_man;

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