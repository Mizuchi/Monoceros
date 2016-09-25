#ifndef ROTATE_TREE_HPP_NXPOF1QZ
#define ROTATE_TREE_HPP_NXPOF1QZ
#include"binary_tree.hpp"

namespace sbl {
namespace detail {
template <class NodePtr> using RotateTreeNodeBase = BinaryTreeNodeBase<NodePtr>;

/// @brief binary tree which support rotate operator.
///
/// Used to be a base class for red-black tree or splay tree.
template <class TExpand, class TUpdate, class TGetNode>
class RotateTreeBase : public BinaryTreeBase<TGetNode> {
public:
  using Expand = TExpand;
  using Update = TUpdate;
  using GetNode = TGetNode;

protected:
  ~RotateTreeBase() {}

private:
  Expand mExpand;
  Update mUpdate;

public:

  using Base = BinaryTreeBase<GetNode>;
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

  RotateTreeBase(const Expand &expand, const Update &update,
                 const GetNode &getNode)
      : Base(getNode), mExpand(expand), mUpdate(update) {}

  template <class NodePtr>
  void expand(NodePtr parent, NodePtr left, NodePtr right) const {
    return mExpand(parent, left, right);
  }

  template <class NodePtr>
  void update(NodePtr parent, NodePtr left, NodePtr right) const {
    return mUpdate(parent, left, right);
  }

  /// expand node x information to his children
  template <class NodePtr> void expand(NodePtr x) const {
    expand(x, get_left(x), get_right(x));
  }

  /// update node x information from his children
  template <class NodePtr> void update(NodePtr x) const {
    update(x, get_left(x), get_right(x));
  }

  /// swap x to her father
  template <class NodePtr> void rotate(NodePtr x) const {
    // CLRS 3rd edition Page 313.
    NodePtr y = get_parent(x);
    if (get_parent(y)) {
      if (get_left(get_parent(y)) == y)
        link_left(get_parent(y), x);
      else
        link_right(get_parent(y), x);
    } else {
      set_parent(x, NodePtr(nullptr));
    }
    size_t c = (get_left(y) == x) ? 0 : 1;
    link_child(y, c, get_child(x, 1 - c));
    link_child(x, 1 - c, y);

    update(y);
    update(x);
  }
};     // class RotateTreeBase
}      // namespace detail
}      // namespace sbl
#endif /* end of include guard: ROTATE_TREE_HPP_NXPOF1QZ */
