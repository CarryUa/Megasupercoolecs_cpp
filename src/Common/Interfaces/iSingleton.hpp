#ifndef MSCE_I_MANAGER_HPP_
#define MSCE_I_MANAGER_HPP_

namespace msce
{
    template <typename Derrived>
    class ISingleton
    {
    public:
        static Derrived *instance;
        virtual ~ISingleton() = default;
    };

    template <typename T>
    T *ISingleton<T>::instance = nullptr;
}

#endif // MSCE_I_MANAGER_HPP_