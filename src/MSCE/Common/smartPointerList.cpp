// #include "smartPointerList.h"

// void msce::SmartUniquePointerList::update_min_dead_index()
// {

//     if (this->min_dead_index_ + 1 == this->list_.size())
//     {
//         this->min_dead_index_++;
//         this->list_.push_back(nullptr);
//         return;
//     }

//     if (this->list_[0] == nullptr)
//     {
//         this->min_dead_index_ = 0;
//         return;
//     }

//     for (size_t i = this->list_.size() - 1; i > 0; i++)
//     {
//         if (this->list_[i] == nullptr)
//         {
//             this->min_dead_index_ = i;
//             return;
//         }
//     }

//     this->min_dead_index_ = this->list_.size();
//     this->list_.push_back(nullptr);
// }

// msce::SmartUniquePointerList::SmartUniquePointerList()
// {
//     this->list_.push_back(nullptr);
// }

// bool msce::SmartUniquePointerList::insert(IHasIntegerIdMember *item)
// {
//     this->list_[min_dead_index_].reset(item);
//     update_min_dead_index();
// }

// bool msce::SmartUniquePointerList::remove(IHasIntegerIdMember *item)
// {
//     return this->remove(item->get_id());
// }

// bool msce::SmartUniquePointerList::remove(size_t index)
// {
//     if (index > this->list_.size())
//         return true;

//     if ()

//         return true;
// }
