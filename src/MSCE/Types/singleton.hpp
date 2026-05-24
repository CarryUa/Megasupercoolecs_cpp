#ifndef MSCE_SINGLETON_HPP_
#define MSCE_SINGLETON_HPP_
#include <iostream>

namespace msce
{
    template <typename Derrived>
    class Singleton
    {
    public:
        inline static Derrived *instance = nullptr;
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