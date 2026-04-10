#include "prototypeManager.h"
#include <fstream>

#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>

using namespace std;

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
        cereal::JSONInputArchive ar(file);
        std::unique_ptr<msce::IPrototype> proto;
        ar(proto);

        this->_prototypes[proto->id] = std::move(proto);
        cout << "Deserialized prototype at '" << path << "'!" << endl;
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
        cereal::JSONOutputArchive ar(file);
        ar(prototype);
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
};