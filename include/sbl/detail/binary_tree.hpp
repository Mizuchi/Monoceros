#ifndef BINARY_TREE_HPP_XYLURV53
#define BINARY_TREE_HPP_XYLURV53
#include <cstddef>
#include <cassert>
#include <queue>
#include <tuple>
#include <iostream>
using std::cout;
using std::endl;

namespace sbl {
namespace detail {
template <class TGetNode> class BinaryTreeBase;
template <class TNodePtr> class BinaryTreeNodeBase {
private:
  using NodePtr = TNodePtr;
  template <class> friend class BinaryTreeBase;
  NodePtr left;   ///< left child
  NodePtr right;  ///< right child
  NodePtr parent; ///< parent node
public:
  // according C++11 § 8.5,p7, these pointers are zero-initialized.
  BinaryTreeNodeBase() : left(), right(), parent() {}

protected:
  ~BinaryTreeNodeBase() {}
};

template <template <class> class BaseNode> class GetNodeDefault {
public:
  template <class NodePtr> auto operator()(NodePtr p) const {
    return static_cast<BaseNode<NodePtr> *>(p);
  }
}; // struct GetNode

struct DoNothing {
  template <class... Args> void operator()(const Args &...) const {}
}; // struct DoNothing

template <class TGetNode> class BinaryTreeBase {
public:
  using GetNode = TGetNode;
  BinaryTreeBase(const GetNode &getNode) : mGetNode(getNode) {}

  /// get the node information from NodePtr
  template <class NodePtr>
  BinaryTreeNodeBase<NodePtr> &get_node(NodePtr a) const {
    assert(a);
    return *mGetNode(a);
  }

  /// get the child according index {0: left, 1: right}
  template <class NodePtr> NodePtr get_child(NodePtr a, size_t idx) const {
    assert(idx == 0 or idx == 1);
    if (idx == 0)
      return get_left(a);
    else
      return get_right(a);
  }

  /// get left child
  template <class NodePtr> NodePtr get_left(NodePtr a) const {
    if (a)
      return get_node(a).left;
    else
      return nullptr;
  }

  /// get right child
  template <class NodePtr> NodePtr get_right(NodePtr a) const {
    if (a)
      return get_node(a).right;
    else
      return nullptr;
  }

  /// get parent
  template <class NodePtr> NodePtr get_parent(NodePtr a) const {
    if (a)
      return get_node(a).parent;
    else
      return nullptr;
  }

private:

  template <class NodePtr>
  void set_child(NodePtr a, size_t idx, NodePtr b) const {
    assert(idx == 0 or idx == 1);
    if (idx == 0)
      set_left(a, b);
    else
      set_right(a, b);
  }

  template <class NodePtr> void set_left(NodePtr a, NodePtr b) const {
    if (a)
      get_node(a).left = b;
  }

  template <class NodePtr> void set_right(NodePtr a, NodePtr b) const {
    if (a)
      get_node(a).right = b;
  }

public:

  /// link the child according index {0: left, 1: right}
  template <class NodePtr>
  void link_child(NodePtr a, size_t idx, NodePtr b) const {
    set_child(a, idx, b);
    set_parent(b, a);
  }

  template <class NodePtr> void link_left(NodePtr a, NodePtr b) const {
    set_left(a, b);
    set_parent(b, a);
  }

  template <class NodePtr> void link_right(NodePtr a, NodePtr b) const {
    set_right(a, b);
    set_parent(b, a);
  }

  /// set parent
  template <class NodePtr> void set_parent(NodePtr a, NodePtr b) const {
    if (a)
      get_node(a).parent = b;
    else
      return;
  }

  /// swap two child of a given node
  template <class NodePtr> void swap_child(NodePtr a) const {
    assert(a);
    NodePtr left = get_left(a);
    NodePtr right = get_right(a);
    link_left(a, right);
    link_right(a, left);
  }

  /// check if x is a right child
  template <class NodePtr> bool is_right(NodePtr x) const {
    assert(x);
    NodePtr y = get_parent(x);
    if (y)
      return get_right(y) == x;
    else
      return false;
  }

  /// check if x is a left child
  template <class NodePtr> bool is_left(NodePtr x) const {
    return not is_right(x);
  }

public:
  template <class NodePtr, class Functor>
  void breadth_first_traversal(NodePtr node, const Functor &functor,
                               size_t depth = 0) const {
    std::queue<std::tuple<NodePtr, size_t> > q = { { node, 0 } };
    while (not q.empty()) {
      std::tie(node, depth) = q.top();
      q.pop();
      functor(node, depth);
      for (NodePtr nextNode : {
        get_left(node), get_right(node)
      }) {
        if (nextNode)
          q.push(std::make_tuple(nextNode, depth + 1));
      }
    }
  }
  template <class NodePtr, class Functor>
  void preorder_traversal(NodePtr node, const Functor &functor,
                          size_t depth = 0) const {
    if (node) {
      functor(node, depth);
      preorder_traversal(get_left(node), functor, depth + 1);
      preorder_traversal(get_right(node), functor, depth + 1);
    }
  }
  template <class NodePtr, class Functor>
  void inorder_traversal(NodePtr node, const Functor &functor,
                         size_t depth = 0) const {
    if (node) {
      inorder_traversal(get_left(node), functor, depth + 1);
      functor(node, depth);
      inorder_traversal(get_right(node), functor, depth + 1);
    }
  }
  template <class NodePtr, class Functor>
  void postorder_traversal(NodePtr node, const Functor &functor,
                           size_t depth = 0) const {
    if (node) {
      postorder_traversal(get_left(node), functor, depth + 1);
      postorder_traversal(get_right(node), functor, depth + 1);
      functor(node, depth);
    }
  }

private:
  GetNode mGetNode;
}; // class BinaryTreeBase

template <class Base> struct Derived : Base {
  using Base::Base;
};
template <template <class...> class Tree, class... Args>
auto make_tree(Args &&... args) {
  return Derived<Tree<Args...> >(std::forward<Args>(args)...);
}

} // namespace detail
using detail::Derived;
struct Less {
  template <class T, class U>
  bool operator()(const T &lhs, const U &rhs) const {
    return lhs < rhs;
  }
}; // struct Less
}      // namespace sbl
#endif /* end of include guard: BINARY_TREE_HPP_XYLURV53 */
