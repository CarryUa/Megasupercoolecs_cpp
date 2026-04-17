#include "prototypeManager.h"
#include <fstream>
#include <functional>
#ifdef DEBUG
#include <cereal/archives/json.hpp>
#endif
#ifdef RELEASE
#include <cereal/archives/binary.hpp>
#endif
#include <cereal/types/memory.hpp>

using namespace std;

msce::Registry<std::string, std::reference_wrapper<const std::type_info>> msce::PrototypeManager::registered_prototypes;
msce::Registry<std::string, std::function<std::unique_ptr<msce::IPrototype>()>> msce::PrototypeManager::registered_factories;

void msce::PrototypeManager::deserialize_prototype(const string &path)
{
    ifstream file(path);
    if (!file)
    {
        cerr << "[PROTO]: " << "Couldn't open prototype file at " << path << endl;
        return;
    }

    try
    {
        std::unique_ptr<msce::IPrototype> proto;

#ifdef DEBUG
        cereal::JSONInputArchive ar(file);
        ar(proto);
#endif
#ifdef RELEASE
        cereal::BinaryInputArchive ar(file);
        ar(proto);
#endif

        this->_prototypes[proto->id] = std::move(proto);
    }
    catch (const exception &e)
    {
        cerr << "[PROTO]: " << "Couldn't deserialize prototype at '" << path << "': " << e.what() << endl;
    }

    file.close();
    return;
}

void msce::PrototypeManager::serialize_prototype(const std::string &path, const unique_ptr<IPrototype> &prototype)
{
    ofstream file(path);
    if (!file)
    {
        cerr << "[PROTO]: " << "Couldn't create prototype cereal file at " << path << endl;
        return;
    }

    try
    {
#ifdef DEBUG
        cereal::JSONOutputArchive ar(file);
        ar(prototype);
#endif
#ifdef RELEASE
        cereal::BinaryOutputArchive ar(file);
        ar(prototype);
#endif
    }
    catch (const exception &e)
    {
        cerr << "[PROTO]: " << "Couldn't serialize '" << prototype->id << "' into file at '" << path << "': " << e.what() << endl;
    }

    file.close();
    return;
}

msce::IPrototype *msce::PrototypeManager::get_prototype(const std::string &id)
{
    if (!this->_prototypes.contains(id))
    {
        cerr << "[PROTO]: " << "Prototype '" << id << "' doesn't exist." << endl;
        return nullptr;
    }

    return this->_prototypes[id].get();
}

unordered_set<msce::IPrototype *> msce::PrototypeManager::enumerate_prototypes()
{
    unordered_set<IPrototype *> result = unordered_set<IPrototype *>();
    for (const auto &[_, p] : this->_prototypes)
    {
        result.insert(p.get());
    }

    return result;
}

msce::IPrototype *msce::PrototypeManager::instantiate_prototype(const std::string &type, const std::string &id) noexcept
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
    _prototypes[id]->id = id;

    return this->_prototypes[id].get();
}
bool msce::PrototypeManager::prototype_id_exists(const std::string &id) const noexcept
{
    return this->_prototypes.contains(id);
}
bool msce::PrototypeManager::prototype_type_registered(const std::string &type) const noexcept
{
    return this->registered_prototypes.is_registered(type) && this->registered_factories.is_registered(type);
}
bool msce::PrototypeManager::delete_prototype(const std::string &id) noexcept
{
    if (!prototype_id_exists(id))
        return false;

    this->_prototypes.erase(id);
    return true;
}
bool msce::PrototypeManager::delete_prototype(IPrototype *proto) noexcept
{
    return delete_prototype(proto->id);
};