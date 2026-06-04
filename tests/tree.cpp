#include <gtest/gtest.h>
#include <test_configs.h>
#include <MSCE/logger.h>
#include <MSCE/Types/Collections/tree.hpp>

using namespace msce;

TEST(TreeTests, NTreeTest)
{
    NTreeNode<int> t1(0);
    NTreeNode<int> t2(1);
    NTreeNode<int> t3(2);
    NTreeNode<int> t4(2);
    NTreeNode<int> t5(3);
    NTreeNode<int> t6(4);

    t1.add_child(&t2);
    t2.add_child(&t3);
    t2.add_child(&t4);
    t3.add_child(&t5);
    t5.add_child(&t6);

    Logger l("TreeTest");

    auto dfs = t1.traverse_dfs();
    for (auto e : dfs)
    {
        l.log_info("{}", e);
    }
}