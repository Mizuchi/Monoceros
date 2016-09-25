#include "sbl/binary_tree.hpp"
#include "gtest/gtest.h"

#include <iostream>
#include <vector>
#include <memory>
#include <string>

TEST(binary_tree, example) {
  struct Node : sbl::BinaryTreeNode<Node *> {
    char c;
    Node(char c) : c(c) {}
  } s[] = { '0', '1', '2', '3', '4' };
  auto bt = sbl::make_binary_tree();
  bt.link_left(&s[0], &s[1]);
  bt.link_right(&s[0], &s[2]);
  bt.link_left(&s[1], &s[3]);
  bt.link_right(&s[2], &s[4]);
  EXPECT_TRUE(bt.is_left(&s[1]));
  EXPECT_TRUE(bt.is_right(&s[2]));
  EXPECT_FALSE(bt.is_right(&s[3]));
  EXPECT_FALSE(bt.is_left(&s[4]));
  std::string ans;
  bt.inorder_traversal(&s[0], [&ans](Node *p, int /*depth*/) { ans += p->c; });
  EXPECT_EQ(ans, "31024");
}
