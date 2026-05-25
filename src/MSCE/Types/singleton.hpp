#ifndef MSCE_SINGLETON_HPP_
#define MSCE_SINGLETON_HPP_
#include <iostream>

namespace msce
{
    /**
     * @brief Automaticaly manages the singleton pattern.
     * @throws Throws std::runtime_error if second instance of singleton object is created.
     * @tparam Derrived Type of derrived object. (AKA object using this)
     */
    template <typename Derrived>
    class Singleton
    {
    public:
        /// @brief Instance of singleton object.
        inline static Derrived *instance = nullptr;
        /// @brief Singleton constructor. Manages the whole thing, trust me bro :)
        /// @throws std::runtime_error if second instance is created.
        Singleton()
        {
            if (instance != nullptr)
            {
                std::string error_msg = "Tried to create new instance of '";
                error_msg += typeid(*this).name();
                error_msg += "', but only 1 is allowed by Singleton base class.";

                throw new std::runtime_error(error_msg);
            }

            instance = static_cast<Derrived *>(this);
        }
    };
}

#endif // MSCE_SINGLETON_HPP_