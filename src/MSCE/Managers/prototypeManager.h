#ifndef MSCE_PROTOTYPE_MANAGER_H_
#define MSCE_PROTOTYPE_MANAGER_H_
#include <vector>
#include <memory>

#include <MSCE/Common/Interfaces/Singleton.hpp>
#include <MSCE/Prototypes/iPrototype.hpp>

namespace msce
{
    class PrototypeManager : Singleton<PrototypeManager>
    {
    private:
        std::vector<std::unique_ptr<IPrototype>> _prototypes;
    };
}

#endif // MSCE_PROTOTYPE_MANAGER_H_