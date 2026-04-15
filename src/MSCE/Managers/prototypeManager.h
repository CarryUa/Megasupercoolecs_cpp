#ifndef MSCE_PROTOTYPE_MANAGER_H_
#define MSCE_PROTOTYPE_MANAGER_H_
#include <memory>
#include <unordered_map>
#include <functional>

#include <MSCE/Common/Interfaces/Singleton.hpp>
#include <MSCE/Common/registry.hpp>
#include <MSCE/Prototypes/prototype.hpp>

namespace msce
{
    class PrototypeManager : public Singleton<PrototypeManager>
    {
    private:
        std::unordered_map<std::string, std::unique_ptr<IPrototype>> _prototypes;

    public:
        static Registry<std::string, std::function<std::unique_ptr<IPrototype>()>> registered_factories;
        static Registry<std::string, std::reference_wrapper<const std::type_info>> registered_prototypes;
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

        template <typename TProto>
        static void register_prototype(const std::string &name);

        /// @brief Creates registered prototype by its typename string at runtime.
        /// @param type The type of the prototype, that it was registered with.
        /// @param id Id that will be asigned to the prototype.
        /// @return Pointer to newly-created prototype or nullptr.
        IPrototype *instantiate_prototype(const std::string &type, const std::string &id) noexcept;

        /// @brief Creates registered prototype by its type-name string at runtime.
        /// @tparam TProto Type that prototype will be cast to.
        /// @param type The type of the prototype, that it was registered with.
        /// @param id Id that will be asigned to the prototype.
        /// @return Pointer to newly-created prototype or nullptr.
        template <typename TProto>
        TProto *instantiate_prototype(const std::string &type, const std::string &id) noexcept;

        /// @param id Try and guess.
        /// @return True if prototype with given ID exists, and false otherwise.
        bool prototype_id_exists(const std::string &id) const noexcept;

        /// @param type Try and guess.
        /// @return True if prototype with given type was registered in bouth regestries, and false otherwise.
        bool prototype_type_registered(const std::string &type) const noexcept;

        /// @param id Try and guess.
        /// @return True if prototype was successfully deleted, and false otherwise.
        bool delete_prototype(const std::string &id) noexcept;

        /// @brief This variant also sets proto argument to nullptr if deleted successfully.
        /// @param proto Pointer to the prototype to be deleted.
        /// @return True if prototype was successfully deleted, and false otherwise.
        bool delete_prototype(IPrototype *proto) noexcept;
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
    std::unordered_set<TProto *> PrototypeManager::enumerate_prototypes()
    {
        std::unordered_set<TProto *> result = std::unordered_set<TProto *>();
        for (const auto &[_, p] : this->_prototypes)
        {
            TProto *cast_p = dynamic_cast<TProto *>(p.get());
            if (cast_p == nullptr)
                continue;

            result.insert(cast_p);
        }

        return result;
    };

    template <typename TProto>
    void PrototypeManager::register_prototype(const std::string &name)
    {
        registered_prototypes.register_entry(name, std::cref(typeid(TProto)));
        registered_factories.register_entry(name,
                                            []()
                                            {
                                                return std::make_unique<TProto>();
                                            });
        std::cout
            << "Registered prototype '" << name << "'" << std::endl;
    }

    template <typename TProto>
    TProto *PrototypeManager::instantiate_prototype(const std::string &type, const std::string &id) noexcept
    {
        if (!registered_factories.is_registered(type))
        {
            std::cerr << "[PROTO]: Error: Prototype with type '" << type << "' wasn't ever registered!" << std::endl;
            return nullptr;
        }
        if (this->_prototypes.contains(id))
        {
            std::cerr << "[PROTO]: Error: Prototype with id '" << id << "' already exist!" << std::endl;
            return nullptr;
        }

        this->_prototypes[id] = registered_factories.get_entry(type)();
        this->_prototypes[id]->id = id;

        auto ptr = dynamic_cast<TProto *>(this->_prototypes[id].get());
        if (ptr == nullptr)
        {
            std::cerr << "[PROTO]: Error: Prototype with type '" << type << "' cant exist!" << std::endl;
            return nullptr;
        }

        return ptr;
    }

    template <typename TProto>
    inline void register_prototype(const std::string &name)
    {
        PrototypeManager::register_prototype<TProto>(name);
    }
}
#endif // MSCE_PROTOTYPE_MANAGER_H_