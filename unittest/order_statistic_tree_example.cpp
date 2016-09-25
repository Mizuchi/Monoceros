#include "sbl/order_statistic_tree.hpp"
#include "gtest/gtest.h"
#include <list>

TEST(structure, OrderStatisticTree) {
  using namespace sbl;
  struct Node : OrderStatisticTreeNode<Node *> {
    int value;
    Node(int _) : value(_) {}
    bool operator<(const Node &rhs) const { return value < rhs.value; }
  };
  std::list<Node> p;
  auto tree = make_order_statistic_tree<Node *>();
  p.push_back(3), tree.insert(&p.back());
  p.push_back(5), tree.insert(&p.back());
  p.push_back(2), tree.insert(&p.back());
  p.push_back(8), tree.insert(&p.back());
  p.push_back(6), tree.insert(&p.back());
  Node x(3);
  EXPECT_EQ(tree.biggest()->value, 8);
  EXPECT_EQ(tree.find_by_order(2)->value, 5);
  EXPECT_EQ(tree.find(&x)->value, 3);
  EXPECT_EQ(tree.bigger(&x)->value, 5);
  EXPECT_EQ(tree.smaller(&x)->value, 2);
  EXPECT_EQ(tree.bigger_count(&x), 3);
  EXPECT_EQ(tree.smaller_count(&x), 1);
  tree.erase(&x);
  EXPECT_EQ(tree.find_by_order(2)->value, 6);
  EXPECT_EQ(tree.biggest()->value, 8);
  EXPECT_EQ(tree.bigger(&x)->value, 5);
  EXPECT_EQ(tree.smaller(&x)->value, 2);
  EXPECT_EQ(tree.bigger_count(&x), 3);
  EXPECT_EQ(tree.smaller_count(&x), 1);
};

