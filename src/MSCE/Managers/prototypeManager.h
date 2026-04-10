#ifndef MSCE_PROTOTYPE_MANAGER_H_
#define MSCE_PROTOTYPE_MANAGER_H_
#include <memory>
#include <unordered_map>

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

        /// @brief Returns the prototype with provided id.
        /// @param id The id of the prototype
        /// @return A pointer to the prototype or nullptr
        IPrototype *get_prototype(const std::string &id);

        /// @brief Returns the prototype with provided id, if it can be cast to the provided type.
        /// @tparam TProto Prototype type to be returned.
        /// @param id The id of the prototype
        /// @return A pointer to the prototype or nullptr
        template <typename TProto>
        TProto *get_prototype(const std::string &id);

        /// @return Returns an unordered set of all prototypes pointers.
        std::unordered_set<IPrototype *> enumerate_prototypes();

        /// @return Returns an unordered set of pointers to all TProto* castable prototypes.
        template <typename TProto>
        std::unordered_set<TProto *> enumerate_prototypes();
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

    template <typename TProto>
    std::unordered_set<TProto *> msce::PrototypeManager::enumerate_prototypes()
    {
        std::unordered_set<TProto *> result = std::unordered_set<TProto *>();
        for (const auto &[_, p] : this->_prototypes)
        {
            TProto *cast_p = dynamic_cast<TProto *>(p);
            if (cast_p == nullptr)
                continue;

            result.insert(cast_p);
        }

        return result;
    };
}
#endif // MSCE_PROTOTYPE_MANAGER_H_