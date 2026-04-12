#ifndef MSCE_I_REGISTRY_HPP
#define MSCE_I_REGISTRY_HPP
#include <unordered_map>
#include <unordered_set>
#include <iostream>

namespace msce
{
    template <class TIdentifyer, class TRegistered>
    class Registry
    {
    protected:
        std::unordered_map<TIdentifyer, TRegistered> registry;

    public:
        void register_entry(TIdentifyer id, const TRegistered &entry)
        {
            if (is_registered(id))
            {
                std::cerr << "[warning]: Double registration detected!" << std::endl;
                return;
            }

            registry.emplace(id, entry);
        }

        /// @exception std::out_of_range - Entry with given id is missing;
        /// @param id The id an entry was registered with.
        /// @return Entry with provided id.
        TRegistered &get_entry(TIdentifyer id)
        {
            return registry.at(id);
        }

        /// @param entry The entry to be checked.
        /// @return True if entry exists.
        inline bool is_registered(const TRegistered &entry)
        {
            for (const auto &[_, e] : registry)
            {
                if (e == entry)
                    return true;
            }
            return false;
        }

        /// @param id The id to be checked.
        /// @return True if entry with provided id exists.
        inline bool is_registered(TIdentifyer id)
        {
            return registry.contains(id);
        }

        /// @return A copy of registry map.
        std::unordered_map<TIdentifyer, TRegistered> enumerate_registry() { return this->registry; }
    };
}

#endif // MSCE_I_REGISTRY_HPP