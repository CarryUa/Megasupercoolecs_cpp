#include <gtest/gtest.h>
#include <test_configs.h>
#include <MSCE/logger.h>
#include <MSCE/Types/Collections/tree.hpp>

using namespace msce;

/// @brief Test basic tree node creation
TEST(TreeTests, NodeCreation)
{
  NTreeNode<int> root(1);
  NTreeNode<int> child(2);

  EXPECT_EQ(root.get_value(), 1);
  EXPECT_EQ(child.get_value(), 2);
}

/// @brief Test adding children to node
TEST(TreeTests, AddChild)
{
  NTreeNode<int> root(1);
  NTreeNode<int> child1(2);
  NTreeNode<int> child2(3);

  root.add_child(&child1);
  root.add_child(&child2);

  EXPECT_EQ(root.get_children().size(), 2);
  EXPECT_EQ(child1.get_parent(), &root);
  EXPECT_EQ(child2.get_parent(), &root);
}

/// @brief Test tree hierarchy
TEST(TreeTests, TreeHierarchy)
{
  NTreeNode<int> root(1);
  NTreeNode<int> level1_1(2);
  NTreeNode<int> level1_2(3);
  NTreeNode<int> level2_1(4);
  NTreeNode<int> level2_2(5);

  root.add_child(&level1_1);
  root.add_child(&level1_2);
  level1_1.add_child(&level2_1);
  level1_1.add_child(&level2_2);

  EXPECT_EQ(root.get_children().size(), 2);
  EXPECT_EQ(level1_1.get_children().size(), 2);
  EXPECT_EQ(level2_1.get_parent(), &level1_1);
}

/// @brief Test depth-first traversal
TEST(TreeTests, DFSTraversal)
{
  NTreeNode<int> root(1);
  NTreeNode<int> child1(2);
  NTreeNode<int> child2(3);
  NTreeNode<int> grandchild(4);

  root.add_child(&child1);
  root.add_child(&child2);
  child1.add_child(&grandchild);

  auto dfs = root.traverse_dfs();
  EXPECT_GE(dfs.size(), 4);
}