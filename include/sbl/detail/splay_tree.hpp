#ifndef SPLAY_TREE_HPP_AL7MKGFX
#define SPLAY_TREE_HPP_AL7MKGFX
#include"rotate_tree.hpp"
namespace sbl {
namespace detail {
template <class NodePtr> using SplayTreeNodeBase = RotateTreeNodeBase<NodePtr>;

template <class TExpand, class TUpdate, class TGetNode>
class SplayTreeBase : public RotateTreeBase<TExpand, TUpdate, TGetNode> {
  // Bottom-Up Splay Tree
  // FAQ
  // Q: Why not use Top-Down Splay Tree?
  // A: There are three reasons.
  //    * Update and expand operator wipe out the performance advantages
  //    * The design of Top-Down is too complex (need 3 trees to splay)
  //    * Since we can not be allocated memory, we have no dummy node
public:
  using Expand = TExpand;
  using Update = TUpdate;
  using GetNode = TGetNode;
  using Base = RotateTreeBase<Expand, Update, GetNode>;
  using Base::get_left;
  using Base::get_right;
  using Base::get_child;
  using Base::get_parent;
  using Base::link_left;
  using Base::link_right;
  using Base::link_child;
  using Base::set_parent;
  using Base::is_left;
  using Base::is_right;
  using Base::get_node;
  using Base::rotate;
  using Base::update;
  using Base::expand;

  SplayTreeBase(const Expand &expand, const Update &update,
                const GetNode &getNode)
      : Base(expand, update, getNode) {}

  /// splay t to the root
  template <class NodePtr> void splay(NodePtr t) const {
    while (get_parent(t)) {
      assert(t);
      NodePtr y = get_parent(t);
      if (not get_parent(y)) {
        rotate(t);
      } else {
        bool isZigZig = is_right(t) and is_right(y);
        bool isZagZag = is_left(t) and is_left(y);
        if (isZigZig or isZagZag) {
          // zig-zig or zag-zag
          rotate(y);
          rotate(t);
        } else {
          // zig-zag or zag-zig
          rotate(t);
          rotate(t);
        }
      }
    }
  }

  /// add a new node to root according compare.
  /// compare must given a NodePtr and return {-1, 0, 1}
  /// -1: current node bigger, to the left subtree
  /// 0: current node equal to x, to the left subtree
  /// 1: current node smaller, to the right subtree
  /// @return new root
  template <class NodePtr, class Compare3way>
  NodePtr add(NodePtr root, NodePtr node, Compare3way compare) const {
    if (not root)
      return node;
    assert(not get_parent(root));
    NodePtr current = root; ///< current node we focus
    while (true) {
      expand(current);
      int k = compare(current);
      if (k == -1 or k == 0) {
        if (not get_left(current)) {
          link_left(current, node);
          break;
        }
        current = get_left(current);
      } else {
        assert(k == 1 and "SplayTree::Compare3way not in {-1, 0, 1}");
        if (not get_right(current)) {
          link_right(current, node);
          break;
        }
        current = get_right(current);
      }
    }
    update(node);
    splay(node);
    return node;
  }

public:

  /// splay the leftmost node
  template <class NodePtr> NodePtr leftmost(NodePtr root) const {
    return find(root, [](NodePtr) {
      return -1;
    });
  }

  /// splay the rightmost node
  template <class NodePtr> NodePtr rightmost(NodePtr root) const {
    return find(root, [](NodePtr) {
      return 1;
    });
  }

  /// delete the root
  /// @return new root
  template <class NodePtr> NodePtr del(NodePtr root) const {
    if (not root)
      return root;
    NodePtr left = get_left(root);
    NodePtr right = get_right(root);
    expand(root);

    // break left child
    link_left(root, NodePtr(nullptr));
    set_parent(left, NodePtr(nullptr));

    // break right child
    link_right(root, NodePtr(nullptr));
    set_parent(right, NodePtr(nullptr));

    if (left) {
      left = rightmost(left);
      assert(not get_right(left));
      link_right(left, right);
      update(left);
      return left;
    } else {
      return right;
    }
  }

  /// find a node according Compare3way compare.
  /// compare must given a NodePtr and return {-1, 0, 1}
  /// -1: to the left subtree
  /// 0: we just found it !!! splay it to the root!
  /// 1: to the right subtree
  /// @return new root (the node we found)
  template <class NodePtr, class Compare3way>
  NodePtr find(NodePtr root, const Compare3way &compare) const {
    if (not root)
      return root;
    NodePtr current = root;
    while (true) {
      assert(current);
      expand(current);
      int k = compare(current);
      if (k == -1) {
        if (not get_left(current))
          break;
        current = get_left(current);
      } else if (k == 1) {
        if (not get_right(current))
          break;
        current = get_right(current);
      } else {
        assert(k == 0 and "SplayTree::Compare3way not in {-1, 0, 1}");
        break;
      }
    }
    splay(current);
    return current;
  }

protected:
  ~SplayTreeBase() {}

};     // class SplayTreeBase
}      // namespace detail
}      // namespace sbl
#endif /* end of include guard: SPLAY_TREE_HPP_AL7MKGFX */
