#include "sbl/vector_tree.hpp"
#include <algorithm>
#include <vector>
#include "gtest/gtest.h"

TEST(structure, vector_tree) {
  using namespace sbl;
  struct Node : VectorTreeNode<Node *> {
    Node(int s) : same(false), value(s), maxValue(s) {}
    bool same;
    int value, maxValue;
  };
  auto t = make_vector_tree([](Node *p, Node *l, Node *r) {
                              if (p->same) {
                                l->value = r->value = p->value;
                                l->same = r->same = p->same;
                                p->same = false;
                              }
                            },
                            [](Node *p, Node *l, Node *r) {
    if (p->same) {
      p->maxValue = p->value;
    } else {
      if (l and r)
        p->maxValue = std::max(l->maxValue, r->maxValue);
      else if (l)
        p->maxValue = l->maxValue;
      else if (r)
        p->maxValue = r->maxValue;
      else
        p->maxValue = p->value;
      p->maxValue = std::max(p->maxValue, p->value);
    }
  });

  Node x[] = { 1, 5, 2, 8, 6, 3, 7 };
  static_cast<void>(x);

  t.insert(0, x, x + 7);
  int value;
  auto getMax = [&value](Node *a) { value = a->maxValue; };
  auto setValue = [](int value) {
    return [value](Node *a) {
      a->value = value;
      a->maxValue = value;
      a->same = true;
    };
  };
  t.call_segment(0, 7, getMax);
  EXPECT_EQ(value, 8);
  t.call_segment(4, 7, getMax);
  EXPECT_EQ(value, 7);
  t.call_segment(0, 3, getMax);
  EXPECT_EQ(value, 5);
  t.call_segment(1, 3, setValue(6));
  t.call_segment(0, 3, getMax);
  EXPECT_EQ(value, 6);
  t.call_segment(1, 5, setValue(2));
  t.call_segment(0, 6, getMax);
  EXPECT_EQ(value, 3);
};
