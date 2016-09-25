#ifndef VECTOR_TREE_HPP_PAAZEMO4
#define VECTOR_TREE_HPP_PAAZEMO4
#include"order_statistic_splay_tree.hpp"

namespace sbl {
namespace detail {

template <class NodePtr>
using VectorTreeNodeBase = OrderStatisticSplayTreeNodeBase<NodePtr>;

template <class Expand, class Update, class GetNode>
class VectorSplayTreeBase
    : public OrderStatisticSplayTreeBase<Expand, Update, GetNode> {
protected:
  ~VectorSplayTreeBase() {}

public:
  using Base = OrderStatisticSplayTreeBase<Expand, Update, GetNode>;
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
  using Base::update;
  using Base::expand;
  using Base::get_size;
  using Base::find_by_order;
  using Base::get_order;

  VectorSplayTreeBase(const Expand &expand, const Update &update,
                      const GetNode &getNode)
      : Base(expand, update, getNode) {}

  template <class NodePtr, class Functor>
  NodePtr call_segment(NodePtr root, size_t l, size_t r,
                       const Functor &func) const {
    NodePtr null(nullptr);
    assert(root);
    assert(get_parent(root) == null);
    assert(l <= r);
    assert(r <= get_size(root));
    if (l == r) {
      func(null);
      return root;
    }
    if (l == 0 and r == get_size(root)) {
      func(root);
    } else if (l == 0) {
      root = find_by_order(root, r);
      func(get_left(root));
      update(root);
    } else if (r == get_size(root)) {
      root = find_by_order(root, l - 1);
      func(get_right(root));
      update(root);
    } else {
      root = find_by_order(root, r);
      expand(root);
      NodePtr newRoot = get_left(root);

      link_left(root, null);
      set_parent(newRoot, null);

      newRoot = find_by_order(newRoot, l - 1);
      expand(newRoot);

      func(get_right(newRoot));

      update(newRoot);
      link_left(root, newRoot);
      update(root);
    }
    return root;
  }

  template <class NodePtr>
  NodePtr replace_segment(NodePtr root, size_t l, size_t r, NodePtr newNode,
                          NodePtr &oldNode) const {
    assert(l < r);
    assert(r <= get_size(root));
    assert(l > 0 or r < get_size(root));
    return call_segment(root, l, r, [newNode, &oldNode, this](NodePtr node) {
      NodePtr parent = get_parent(node);
      assert(parent);
      if (is_left(node))
        link_left(parent, newNode);
      else
        link_right(parent, newNode);
      oldNode = node;
    });
  }
};     // class VectorTree
}      // namespace detail
}      // namespace sbl
#endif /* end of include guard:  */
