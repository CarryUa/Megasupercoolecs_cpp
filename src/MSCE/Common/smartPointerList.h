#ifndef MSCE_SMART_POINTER_LIST_
#define MSCE_SMART_POINTER_LIST_
#include <vector>
#include <memory>
#include <type_traits>

#include <MSCE/Common/iHasIdMember.h>

namespace msce
{
    /**
     * @brief Extends std::vector functionality to sparingly use and reuse std::unique_ptr.
     */
    template <class TPointedTo>
    struct SmartUniquePointerList
    {
        static_assert(std::is_base_of_v<IHasIntegerIdMember, TPointedTo>,
                      "TPointedTo concrete needs to inherit from msce::IHasIntegerIdMember.");

    private:
        std::vector<std::unique_ptr<TPointedTo>> list_;
        size_t min_dead_index_ = 0;
        size_t alive_items_count_ = 0;

        void update_min_dead_index();

    public:
        struct iterator
        {
        private:
            SmartUniquePointerList &collection_ref_;
            size_t index_;

        public:
            iterator(SmartUniquePointerList &collection_ref, size_t index);

            TPointedTo *operator*();
            iterator &operator++();
            bool operator!=(const iterator &other);
        };

        iterator begin()
        {
            return iterator(*this, 0);
        }

        iterator end()
        {
            return iterator(*this, this->size());
        }

        SmartUniquePointerList();

        bool insert(TPointedTo *item);
        bool remove(TPointedTo *item);
        bool remove(size_t index);
        bool has_item(TPointedTo *item);
        bool is_alive(size_t index);

        size_t size() const noexcept;
        size_t alive_count() const noexcept;

        TPointedTo *get_item(size_t index) const;
        // template <typename T>
        // TPointedTo *clone_item(T *original);
        template <typename T>
        std::vector<T> get_all_items_of_type();

        TPointedTo *operator[](size_t index);
    };
    template <class TPointedTo>
    inline void SmartUniquePointerList<TPointedTo>::update_min_dead_index()
    {
        if (this->min_dead_index_ + 1 == this->list_.size())
        {
            this->min_dead_index_++;
            this->list_.push_back(nullptr);
            return;
        }

        if (this->list_[0] == nullptr)
        {
            this->min_dead_index_ = 0;
            return;
        }

        for (size_t i = this->list_.size() - 1; i > 0; i++)
        {
            if (this->list_[i] == nullptr)
            {
                this->min_dead_index_ = i;
                return;
            }
        }

        this->min_dead_index_ = this->list_.size();
        this->list_.push_back(nullptr);
    }

    template <class TPointedTo>
    inline SmartUniquePointerList<TPointedTo>::SmartUniquePointerList()
    {
        this->list_.push_back(nullptr);
    }
    template <class TPointedTo>
    inline bool SmartUniquePointerList<TPointedTo>::insert(TPointedTo *item)
    {
        if (item == nullptr)
            return false;

        if (this->has_item(item))
            return true;

        this->list_[min_dead_index_].reset(item);
        dynamic_cast<IHasIntegerIdMember *>(item)->set_id(min_dead_index_);
        alive_items_count_++;

        this->update_min_dead_index();
        return true;
    }
    template <class TPointedTo>
    inline bool SmartUniquePointerList<TPointedTo>::remove(TPointedTo *item)
    {
        return remove(item->get_id());
    }
    template <class TPointedTo>
    inline bool SmartUniquePointerList<TPointedTo>::remove(size_t index)
    {
        if (!is_alive(index))
            return true;

        this->list_[index].reset();
        alive_items_count_--;
        min_dead_index_ = index;
        return true;
    }
    template <class TPointedTo>
    inline bool SmartUniquePointerList<TPointedTo>::has_item(TPointedTo *item)
    {
        if (!is_alive(item->get_id()))
            return false;

        return this->list_[item->get_id()].get() == item;
    }
    template <class TPointedTo>
    inline bool SmartUniquePointerList<TPointedTo>::is_alive(size_t index)
    {
        if (index > this->list_.size())
            return false;

        return this->list_[index] != nullptr;
    }
    template <class TPointedTo>
    inline size_t SmartUniquePointerList<TPointedTo>::size() const noexcept
    {
        return this->list_.size();
    }
    template <class TPointedTo>
    inline size_t SmartUniquePointerList<TPointedTo>::alive_count() const noexcept
    {
        return this->alive_items_count_;
    }
    template <class TPointedTo>
    inline TPointedTo *SmartUniquePointerList<TPointedTo>::get_item(size_t index) const
    {
        if (index > this->list_.size())
            return nullptr;

        return this->list_[index].get();
    }
    template <class TPointedTo>
    inline TPointedTo *SmartUniquePointerList<TPointedTo>::operator[](size_t index)
    {
        return this->get_item(index);
    }
    template <class TPointedTo>
    template <typename T>
    inline std::vector<T> SmartUniquePointerList<TPointedTo>::get_all_items_of_type()
    {
        std::vector<T> result;
        for (auto *item : this->list_)
        {
            T *cast = dynamic_cast<T *>(item);
            if (cast == nullptr)
                continue;

            result.push_back(cast);
        }
        return result;
    }
    template <class TPointedTo>
    inline SmartUniquePointerList<TPointedTo>::iterator::iterator(SmartUniquePointerList &collection_ref, size_t index)
        : collection_ref_(collection_ref), index_(index)
    {
        while (collection_ref_[index] == nullptr && index < collection_ref_.size())
            index++;
    }
    template <class TPointedTo>
    inline TPointedTo *SmartUniquePointerList<TPointedTo>::iterator::operator*()
    {
        return collection_ref_[index_];
    }
    template <class TPointedTo>
    inline bool SmartUniquePointerList<TPointedTo>::iterator::operator!=(const iterator &other)
    {
        return this->index_ != other.index_;
    }
    template <class TPointedTo>
    inline SmartUniquePointerList<TPointedTo>::iterator &SmartUniquePointerList<TPointedTo>::iterator::operator++()
    {
        do
        {
            this->index_++;
        } while (this->collection_ref_[this->index_] == nullptr && this->index_ < this->collection_ref_.size());
        return *this;
    }

}
#endif // MSCE_SMART_POINTER_LIST_