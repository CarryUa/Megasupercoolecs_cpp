#ifndef MSCE_SMART_UNIQUE_POINTER_LIST_
#define MSCE_SMART_UNIQUE_POINTER_LIST_
#include <vector>
#include <memory>

#include <MSCE/Types/Collections/handle.hpp>

#define LocalHandle SmartHandle<SmartUniquePointerList<TPointedTo>, TPointedTo>

namespace msce
{
    /**
     * @brief Extends std::vector functionality to sparingly use and reuse std::unique_ptr.
     * Use this when you need to be able to delete items.
     *
     * @note This is "owning" list.
     * @note Almost Always try to use iterators. They do not return nullptrs.
     *
     * @tparam TPointedTo the type of objects stored.
     */
    template <class TPointedTo>
    struct SmartUniquePointerList
    {
    private:
        std::vector<std::unique_ptr<TPointedTo>> list_;
        std::vector<std::uint32_t> coresponding_generations_;
        uint32_t min_dead_index_ = 0;
        uint32_t alive_items_count_ = 0;

        /**
         *  Updates the @see msce::SmartUniquePointerList::min_dead_index_.
         *
         *  @note If the index is in the end of the list(eg. after multiple sequencial inserts), its time complexity will be @b O(1).
         *  @note Otherwise it will be @b O(n).
         */
        void update_min_dead_index();

    public:
        struct Iterator
        {
        private:
            const SmartUniquePointerList &collection_ref_;
            uint32_t index_;

        public:
            Iterator(const SmartUniquePointerList &collection_ref, uint32_t index);

            LocalHandle operator*() const noexcept;
            Iterator &operator++();
            bool operator!=(const Iterator &other) const;
        };

        Iterator begin() const
        {
            return Iterator(*this, 0);
        }

        Iterator end() const
        {
            return Iterator(*this, this->size());
        }

        SmartUniquePointerList();

        bool insert(LocalHandle item) noexcept;
        LocalHandle insert(TPointedTo *) noexcept;
        bool remove(LocalHandle item) noexcept;
        bool remove(uint32_t index) noexcept;
        bool is_alive(uint32_t index) const noexcept;
        TPointedTo *resolve(uint32_t index, uint32_t generation) const noexcept;

        uint32_t size() const noexcept;
        uint32_t alive_count() const noexcept;
        LocalHandle get_item(uint32_t index) const noexcept;
        // template <typename T>
        // TPointedTo *clone_item(T *original);
        template <typename T>
        std::vector<SmartHandle<SmartUniquePointerList<TPointedTo>, T>> get_all_items_of_type() const noexcept;

        LocalHandle operator[](uint32_t index) const;
    };
    template <class TPointedTo>
    inline void SmartUniquePointerList<TPointedTo>::update_min_dead_index()
    {
        if (this->min_dead_index_ + 1 == this->list_.size())
        {
            this->min_dead_index_++;
            return;
        }

        for (uint32_t i = 0; i < this->list_.size(); i++)
        {
            if (this->list_[i] == nullptr)
            {
                this->min_dead_index_ = i;
                return;
            }
        }

        this->min_dead_index_ = this->list_.size();
    }

    template <class TPointedTo>
    inline SmartUniquePointerList<TPointedTo>::SmartUniquePointerList()
    {
        this->list_.push_back(nullptr);
        this->coresponding_generations_.push_back(0);
    }
    template <class TPointedTo>
    inline bool SmartUniquePointerList<TPointedTo>::insert(LocalHandle item) noexcept
    {
        if (item == nullptr)
            return false;

        if (this->min_dead_index_ == this->list_.size())
        {
            this->list_.push_back(nullptr);
            this->coresponding_generations_.push_back(0);
            this->list_.back().reset(item);
        }
        else
        {
            this->list_[min_dead_index_].reset(item.get());
            this->coresponding_generations_[min_dead_index_]++;
        }
        alive_items_count_++;

        this->update_min_dead_index();
        return true;
    }
    template <class TPointedTo>
    inline LocalHandle SmartUniquePointerList<TPointedTo>::insert(TPointedTo *item) noexcept
    {
        if (item == nullptr)
            return LocalHandle::create_nullptr();

        if (this->min_dead_index_ == this->list_.size())
        {
            this->list_.push_back(nullptr);
            this->coresponding_generations_.push_back(0);
            this->list_.back().reset(item);
        }
        else
        {
            this->list_[min_dead_index_].reset(item);
            this->coresponding_generations_[min_dead_index_]++;
        }
        alive_items_count_++;

        auto handle = this->get_item(min_dead_index_);
        this->update_min_dead_index();
        return handle;
    }
    template <class TPointedTo>
    inline bool SmartUniquePointerList<TPointedTo>::remove(LocalHandle item) noexcept
    {
        return remove(item.get_index());
    }
    template <class TPointedTo>
    inline bool SmartUniquePointerList<TPointedTo>::remove(uint32_t index) noexcept
    {
        if (!is_alive(index))
            return true;

        this->list_[index].reset(nullptr);
        alive_items_count_--;
        min_dead_index_ = index;
        return true;
    }

    template <class TPointedTo>
    inline bool SmartUniquePointerList<TPointedTo>::is_alive(uint32_t index) const noexcept
    {
        if (index >= this->list_.size())
            return false;

        return this->list_[index] != nullptr;
    }
    template <class TPointedTo>
    inline TPointedTo *SmartUniquePointerList<TPointedTo>::resolve(uint32_t index, uint32_t generation) const noexcept
    {
        if (!is_alive(index))
            return nullptr;

        if (this->coresponding_generations_[index] != generation)
            return nullptr;

        return this->list_[index].get();
    }
    template <class TPointedTo>
    inline uint32_t SmartUniquePointerList<TPointedTo>::size() const noexcept
    {
        return this->list_.size();
    }
    template <class TPointedTo>
    inline uint32_t SmartUniquePointerList<TPointedTo>::alive_count() const noexcept
    {
        return this->alive_items_count_;
    }
    template <class TPointedTo>
    inline SmartHandle<SmartUniquePointerList<TPointedTo>, TPointedTo> SmartUniquePointerList<TPointedTo>::get_item(uint32_t index) const noexcept
    {
        if (index > this->list_.size())
            return LocalHandle::create_nullptr();

        return LocalHandle(index, this->coresponding_generations_[index], this);
    }
    template <class TPointedTo>
    inline SmartHandle<SmartUniquePointerList<TPointedTo>, TPointedTo> SmartUniquePointerList<TPointedTo>::operator[](uint32_t index) const
    {
        return this->get_item(index);
    }
    template <class TPointedTo>
    template <typename T>
    inline std::vector<SmartHandle<SmartUniquePointerList<TPointedTo>, T>> SmartUniquePointerList<TPointedTo>::get_all_items_of_type() const noexcept
    {
        std::vector<SmartHandle<SmartUniquePointerList<TPointedTo>, T>> result;
        Iterator end = this->end();
        Iterator cur = this->begin();
        for (; cur != end; ++cur)
        {
            LocalHandle handle = *(cur);
            T *cast = dynamic_cast<T *>(handle.get());
            if (cast == nullptr)
                continue;

            result.push_back(static_cast<SmartHandle<SmartUniquePointerList<TPointedTo>, T>>(handle));
        }
        return result;
    }
    template <class TPointedTo>
    inline SmartUniquePointerList<TPointedTo>::Iterator::Iterator(const SmartUniquePointerList &collection_ref, uint32_t index)
        : collection_ref_(collection_ref), index_(index)
    {
        while (collection_ref_[index_] == nullptr && index_ < collection_ref_.size())
            index_++;
    }
    template <class TPointedTo>
    inline SmartHandle<SmartUniquePointerList<TPointedTo>, TPointedTo> SmartUniquePointerList<TPointedTo>::Iterator::operator*() const noexcept
    {
        return collection_ref_[index_];
    }
    template <class TPointedTo>
    inline bool SmartUniquePointerList<TPointedTo>::Iterator::operator!=(const Iterator &other) const
    {
        return this->index_ != other.index_;
    }
    template <class TPointedTo>
    inline SmartUniquePointerList<TPointedTo>::Iterator &SmartUniquePointerList<TPointedTo>::Iterator::operator++()
    {
        do
        {
            this->index_++;
        } while (this->index_ < this->collection_ref_.size() && this->collection_ref_[this->index_].get() == nullptr);
        return *this;
    }

}
#endif // MSCE_SMART_UNIQUE_POINTER_LIST_