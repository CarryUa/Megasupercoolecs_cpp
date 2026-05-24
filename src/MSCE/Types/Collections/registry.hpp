#ifndef MSCE_I_REGISTRY_HPP
#define MSCE_I_REGISTRY_HPP
#include <MSCE/logger.h>

#include <unordered_map>
#include <unordered_set>
#include <iostream>

namespace msce
{
    template <class TIdentifyer, class TRegistered>
    class Registry
    {
    protected:
        std::unordered_map<TIdentifyer, TRegistered> registry_;

    public:
        Registry()
        {
            static Logger logger("Registry");
            logger.log_debug("New registry was created");
        }

        void register_entry(TIdentifyer id, const TRegistered &entry)
        {
            static Logger logger("Registry");
            if (is_registered(id))
            {
                logger.log_warning("Detected double registry of a prototype! Ignoring...");
                return;
            }
            registry_.emplace(id, entry);
        }

        /// @exception std::out_of_range - Entry with given id is missing;
        /// @param id The id an entry was registered with.
        /// @return Entry with provided id.
        TRegistered &get_entry(TIdentifyer id)
        {
            return registry_.at(id);
        }

        /// @param entry The entry to be checked.
        /// @return True if entry exists.
        inline bool is_registered(const TRegistered &entry)
        {
            for (const auto &[_, e] : registry_)
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
            return registry_.contains(id);
        }

        /// @return A copy of registry map.
        std::unordered_map<TIdentifyer, TRegistered> enumerate_registry() { return this->registry_; }
    };
}

#endif // MSCE_I_REGISTRY_HPP