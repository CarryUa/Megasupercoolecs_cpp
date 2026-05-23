#ifndef MSCE_SmartHandle_HPP_
#define MSCE_SmartHandle_HPP_

namespace msce
{
    /**
     * Handle pattern for SmartPointerLists.
     */
    template <typename TCollection, typename TItem>
    class SmartHandle
    {
    private:
        const TCollection *smart_collection_ptr_;
        /**
         * The SmartHandle id consists of 2 numbers stored separately in its first and second 32 bits respectively.
         * The first number is 32bit unsigned integer representing the index inside collection.
         * The second number is 32bit unsigned integer representing generation of collection's index at SmartHandle creation time.
         *
         * @note Value UINT64_MAX is treated as null.
         */
        uint64_t handle_id_ = UINT64_MAX;

    public:
        static SmartHandle create_nullptr() noexcept;
        SmartHandle(uint32_t index, uint32_t generation, const TCollection *collection);

        uint32_t get_generation() const noexcept;
        uint32_t get_index() const noexcept;
        /**
         * Returns the pointer to underlying object or nullptr.
         */
        TItem *get() const noexcept;
        TItem *operator->() const noexcept;

        bool operator==(const SmartHandle<TCollection, TItem> &rhs) const noexcept;
        bool operator==(const std::nullptr_t rhs) const noexcept;

        bool operator!=(const SmartHandle<TCollection, TItem> &rhs) const noexcept;
        bool operator!=(const std::nullptr_t rhs) const noexcept;

        /**
         * @note Notice that you can't cast collection type. This is intentional, so handlers won't switch collections and create invalid pointers.
         */
        template <typename NewTItem>
        explicit operator SmartHandle<TCollection, NewTItem>() const noexcept;
    };
    template <typename TCollection, typename TItem>
    inline TItem *SmartHandle<TCollection, TItem>::get() const noexcept
    {
        if (this->smart_collection_ptr_ == nullptr)
            return nullptr;
        if (handle_id_ == UINT64_MAX)
            return nullptr;

        TItem *item = dynamic_cast<TItem *>(smart_collection_ptr_->resolve(get_index(), get_generation()));
        return item;
    }
    template <typename TCollection, typename TItem>
    inline TItem *SmartHandle<TCollection, TItem>::operator->() const noexcept
    {
        return get();
    }
    template <typename TCollection, typename TItem>
    inline bool SmartHandle<TCollection, TItem>::operator==(const SmartHandle<TCollection, TItem> &rhs) const noexcept
    {
        return handle_id_ == rhs.handle_id_ && smart_collection_ptr_ == rhs.smart_collection_ptr_;
    }
    template <typename TCollection, typename TItem>
    inline bool SmartHandle<TCollection, TItem>::operator==(const std::nullptr_t rhs) const noexcept
    {
        return get() == rhs;
    }
    template <typename TCollection, typename TItem>
    inline bool SmartHandle<TCollection, TItem>::operator!=(const SmartHandle<TCollection, TItem> &rhs) const noexcept
    {
        return handle_id_ != rhs.handle_id_ || smart_collection_ptr_ != rhs.smart_collection_ptr_;
    }
    template <typename TCollection, typename TItem>
    inline bool SmartHandle<TCollection, TItem>::operator!=(const std::nullptr_t rhs) const noexcept
    {
        return get() != rhs;
    }
    template <typename TCollection, typename TItem>
    inline SmartHandle<TCollection, TItem> SmartHandle<TCollection, TItem>::create_nullptr() noexcept
    {
        return SmartHandle(UINT32_MAX, UINT32_MAX, nullptr);
    }
    template <typename TCollection, typename TItem>
    inline SmartHandle<TCollection, TItem>::SmartHandle(uint32_t index, uint32_t generation, const TCollection *collection)
    {
        smart_collection_ptr_ = collection;
        handle_id_ = (static_cast<uint64_t>(index) << 32) | static_cast<uint64_t>(generation);
    }
    template <typename TCollection, typename TItem>
    inline uint32_t SmartHandle<TCollection, TItem>::get_generation() const noexcept
    {
        return static_cast<uint32_t>(handle_id_);
    }
    template <typename TCollection, typename TItem>
    inline uint32_t SmartHandle<TCollection, TItem>::get_index() const noexcept
    {
        return static_cast<uint32_t>(handle_id_ >> 32);
    }
    template <typename TCollection, typename TItem>
    template <typename NewTItem>
    inline SmartHandle<TCollection, TItem>::operator SmartHandle<TCollection, NewTItem>() const noexcept
    {
        return SmartHandle<TCollection, NewTItem>(this->get_index(), this->get_generation(), this->smart_collection_ptr_);
    }
}

#endif // MSCE_SmartHandle_HPP_