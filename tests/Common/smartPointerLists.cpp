#include <gtest/gtest.h>
#include <test_configs.h>

#include <MSCE/Common/smartUniquePointerList.hpp>

using namespace msce;
using namespace std;

namespace
{
    struct Object
    {
        ulong id;
        int integer;
        bool boolean;
        string str;

        Object()
        {
            integer = rand();
            boolean = rand() % 2 == 0;
            str = string("Hello World").substr(0, rand() % 12);
        }

        virtual ~Object() = default;
    };
    struct DerivedObject : public Object
    {
        float floating_point;
        DerivedObject()
        {
            floating_point = static_cast<double>(rand()) / 5.0;
        }
    };
}

TEST(SmartPointerListTests, UniquePointerListTest)
{
    SmartUniquePointerList<Object> list;

    // populate
    for (size_t i = 0; i < TEST_ITERATIONS; i++)
    {
        if (i % 2 == 0)
            list.insert(new Object());
        else
            list.insert(new DerivedObject());
    }
    ASSERT_EQ(list.size(), TEST_ITERATIONS);
    ASSERT_EQ(list.alive_count(), TEST_ITERATIONS);

    // test reusing of empty slots.
    size_t rand_i = rand() % TEST_ITERATIONS;
    auto item = list.get_item(rand_i);
    EXPECT_TRUE(list.is_alive(item.get_index()));
    list.remove(rand_i);
    EXPECT_EQ(list.get_item(rand_i).get(), nullptr) << "Item was not deleted!";

    EXPECT_EQ(list.alive_count(), TEST_ITERATIONS - 1);
    EXPECT_FALSE(list.is_alive(rand_i));

    auto new_item = new Object();

    auto new_item_handle = list.insert(new_item);
    EXPECT_EQ(new_item_handle.get_index(), rand_i) << "The empty id " << rand_i << " wasn't properly reused.";
    EXPECT_EQ(list.size(), TEST_ITERATIONS) << "The list changed size instead of reusing availible.";
    EXPECT_EQ(list.alive_count(), TEST_ITERATIONS);

    // test iteration
    for (auto item : list)
    {
        EXPECT_NE(item, nullptr) << "Iterator returned a nullptr, which souldn't happen.";
        EXPECT_TRUE(list.is_alive(item.get_index())) << "wtf?!";
        if (!list.is_alive(item.get_index()))
            break;
        // No point in polluting the output. Iterators should iterate only trough alive items.
    }

    auto derived_vec = list.get_all_items_of_type<DerivedObject>();
    for (auto derived : derived_vec)
    {
        EXPECT_NE(derived, nullptr) << "Iterator returned a nullptr, which souldn't happen.";
        EXPECT_TRUE(list.is_alive(derived.get_index())) << "Object in derived slice of the list isn't inside the original";
        if (!list.is_alive(derived.get_index()))
            break;
        // No point in polluting the output. Iterators should iterate only trough alive items.
    }
}
