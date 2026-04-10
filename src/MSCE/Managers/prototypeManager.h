#ifndef MSCE_PROTOTYPE_MANAGER_H_
#define MSCE_PROTOTYPE_MANAGER_H_
#include <unordered_map>
#include <memory>

#include <MSCE/Common/Interfaces/Singleton.hpp>
#include <MSCE/Prototypes/prototype.hpp>

namespace msce
{
    class PrototypeManager : public Singleton<PrototypeManager>
    {
    private:
        std::unordered_map<std::string, std::unique_ptr<IPrototype>> _prototypes;

    public:
        /// @brief Tries do deserialize cereal file and create a prototype based on it.
        /// @param path The path to cereal file.
        void deserialize_prototype(const std::string &path);

        /// @brief Tries do serialize a prototype.
        /// @param path The path to cereal file to be created.
        /// @param prototype The prototype uptr that needs to be serialized.
        void serialize_prototype(const std::string &path, const std::unique_ptr<IPrototype> &prototype);

        IPrototype *get_prototype(const std::string &id);

        template <typename TProto>
        TProto *get_prototype(const std::string &id);
    };

    template <typename TProto>
    TProto *PrototypeManager::get_prototype(const std::string &id)
    {
        if (!this->_prototypes.contains(id))
        {
            std::cerr << "[PROTO]: Prototype with id '" << id << "' doesn't exist" << std::endl;
            return nullptr;
        }
        TProto *p = dynamic_cast<TProto *>(this->_prototypes[id].get());
        if (p == nullptr)
        {
            std::cerr << "[PROTO]: Prototype with id '" << id << "' doesn't match type provided: " << typeid(TProto).name() << std::endl;
        }

        return p;
    }
}
#endif // MSCE_PROTOTYPE_MANAGER_H_