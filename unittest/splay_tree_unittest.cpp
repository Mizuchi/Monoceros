#include "sbl/splay_tree.hpp"
#include "gtest/gtest.h"

#include <iostream>
#include <vector>
#include <memory>
#include <string>

TEST(splay_tree, zigzigandzigzag) {
  struct Node : sbl::SplayTreeNode<Node *> {
    char c;
  } s[10]; // struct Node
  for (auto &i : s) {
    static char c = '0';
    i.c = c++;
  }
  auto bt = sbl::make_splay_tree();
  bt.link_left(&s[0], &s[1]);
  bt.link_right(&s[0], &s[2]);
  bt.link_left(&s[2], &s[3]);
  bt.link_right(&s[2], &s[4]);
  bt.link_left(&s[3], &s[5]);
  bt.link_right(&s[3], &s[6]);
  bt.link_left(&s[5], &s[7]);
  bt.link_right(&s[5], &s[8]);
  bt.link_left(&s[7], &s[9]);
  std::string ss;
  bt.inorder_traversal(&s[0], [&ss](Node *p, int /*depth*/) { ss += p->c; });
  EXPECT_EQ(ss, "1097583624");
  bt.splay(&s[7]);
  ss = "";
  bt.inorder_traversal(&s[7], [&ss](Node *p, int /*depth*/) { ss += p->c; });
  EXPECT_EQ(ss, "1097583624");
  ss = "";
  bt.preorder_traversal(&s[7], [&ss](Node *p, int /*depth*/) { ss += p->c; });
  EXPECT_EQ(ss, "7019253864");
  ss = "";
  bt.postorder_traversal(&s[7], [&ss](Node *p, int /*depth*/) { ss += p->c; });
  EXPECT_EQ(ss, "1908635427");
}

