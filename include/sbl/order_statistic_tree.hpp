#ifndef ORDER_STATISTIC_TREE_HPP_H6UQPNLS
#define ORDER_STATISTIC_TREE_HPP_H6UQPNLS
#include <cstddef>
#include <cstdlib>
#include <cassert>
#include "detail/order_statistic_splay_tree.hpp"
#include "function_traits.hpp"

namespace sbl {
namespace detail {
/// @brief order statistic tree is a data structure described in Introduction
/// to Algorithm.
///
/// This is an augmenting data structure, which add size attribute to every
/// tree nodes. Size attribute represent how many nodes in current subtree
/// (treat the node we focus as root). For more information, read CLRS Chapter
/// 14.
///
/// @tparam NodePtr a pointer to the node which defined by user.
template <class NodePtr, class Tree, class Compare> class OrderStatisticTree {
  // This data structure implement by using Splay Tree.
  // Splay Tree is a binary search tree.
private:
  Tree tree;
  const Compare &compare;
  mutable NodePtr root;

  int three_way(NodePtr x, NodePtr y) const {
    if (compare(x, y))
      return -1;
    if (compare(y, x))
      return +1;
    return 0;
  }

public:

  OrderStatisticTree(const Tree &tree, const Compare &compare)
      : tree(tree), compare(compare), root() {}

  /// @return number of all nodes.
  size_t size() const { return tree.get_size(root); }

  /// add a new node to root
  void insert(NodePtr x) {
    assert(x);
    root = tree.add(root, x, [&x, this](NodePtr y) {
      return three_way(x, y);
    });
  }

  /// @return the node has been removed.
  NodePtr erase(NodePtr x) {
    auto compare = [&x, this](NodePtr y) { return three_way(x, y); }
    ;
    root = tree.find(root, compare);
    if (compare(root) == 0) {
      NodePtr delNode = root;
      root = tree.del(root);
      return delNode;
    }
    return nullptr;
  }

  /// @return number of nodes that smaller than NodePtr x.
  size_t smaller_count(NodePtr x) const {
    assert(x);
    if (smaller(x) == nullptr)
      return 0;
    NodePtr leftChild = tree.get_left(root);
    return tree.get_size(leftChild) + 1;
  }

  /// @return number of nodes that bigger than NodePtr x.
  size_t bigger_count(NodePtr x) const {
    assert(x);
    if (bigger(x) == nullptr)
      return 0;
    NodePtr rightChild = tree.get_right(root);
    return tree.get_size(rightChild) + 1;
  }

public:

  /// @return first NodePtr smaller than NodePtr x.
  NodePtr smaller(NodePtr x) const {
    auto smaller3Way = [&x, this](NodePtr current) {
      if (compare(current, x))
        return 1;
      return -1;
    }
    ;
    root = tree.find(root, smaller3Way);
    assert(x);
    assert(root);
    if (compare(root, x)) {
      return root;
    } else {
      // half change to splay the bigger one.
      root = tree.find(root, smaller3Way);
      assert(x);
      assert(root);
      if (compare(root, x))
        return root;
      else
        return nullptr;
    }
    assert(false);
  }

  /// @return first NodePtr bigger than NodePtr x.
  NodePtr bigger(NodePtr x) const {
    auto bigger3Way = [&x, this](NodePtr current) {
      if (compare(x, current))
        return -1;
      return 1;
    }
    ;
    root = tree.find(root, bigger3Way);
    assert(x);
    assert(root);
    if (compare(x, root)) {
      return root;
    } else {
      // half change to splay the smaller one.
      root = tree.find(root, bigger3Way);
      assert(x);
      assert(root);
      if (compare(x, root))
        return root;
      else
        return nullptr;
    }
    assert(false);
  }

  /// @return the minimum node.
  NodePtr smallest() const {
    root = tree.leftmost(root);
    assert(tree.get_left(root) == nullptr);
    return root;
  }
  /// @return the maximum node.
  NodePtr biggest() const {
    root = tree.rightmost(root);
    assert(tree.get_right(root) == nullptr);
    return root;
  }

  /// @return n-th node
  NodePtr find_by_order(size_t n) const {
    root = tree.find_by_order(root, n);
    return root;
  }

  /// @return equivalence node of x.
  NodePtr find(NodePtr x) const {
    root = tree.find(root, [&x, this](NodePtr y) {
      return three_way(x, y);
    });
    return root;
  }

  /// @return remove all node in OrderStatisticTree
  void clear() { root = nullptr; }
};

template <class NodePtr>
using OrderStatisticTreeNode =
    Derived<OrderStatisticSplayTreeNodeBase<NodePtr> >;

template <class NodePtr, class Compare,
          class GetNode = GetNodeDefault<OrderStatisticSplayTreeNodeBase> >
auto make_order_statistic_tree(const Compare &compare,
                            const GetNode &getNode = GetNode()) {
  using namespace detail;
  auto tree =
      make_tree<OrderStatisticSplayTreeBase>(DoNothing(), DoNothing(), getNode);
  return OrderStatisticTree<NodePtr, decltype(tree), Compare>(tree, compare);
}

template <class Compare,
          class GetNode = GetNodeDefault<OrderStatisticSplayTreeNodeBase> >
auto make_order_statistic_tree(const Compare &compare,
                            const GetNode &getNode = GetNode()) {
  using NodePtr = typename function_traits<Compare>::template arg<0>::type;
  return make_order_statistic_tree<NodePtr>(compare, getNode);
}

template <class NodePtr>
auto make_order_statistic_tree() {
  return make_order_statistic_tree([](NodePtr lhs, NodePtr rhs) {
    return *lhs < *rhs;
  });
}
} // namespace detail
using detail::make_order_statistic_tree;
using detail::OrderStatisticTreeNode;
}      // namespace sbl
#endif /* end of include guard: ORDER_STATISTIC_TREE_HPP_H6UQPNLS */
