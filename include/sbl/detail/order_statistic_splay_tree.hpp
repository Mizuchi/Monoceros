#ifndef ORDER_STATISTIC_SPLAY_TREE_HPP_CBTKNGFR
#define ORDER_STATISTIC_SPLAY_TREE_HPP_CBTKNGFR
#include"splay_tree.hpp"

namespace sbl {
namespace detail {
namespace ost { template <class GetNode> class BaseAlgo; }

template <class NodePtr>
class OrderStatisticSplayTreeNodeBase : public SplayTreeNodeBase<NodePtr> {
private:
  template <class> friend class ost::BaseAlgo;
  size_t size;

public:
  OrderStatisticSplayTreeNodeBase() : size(1) {}

protected:
  ~OrderStatisticSplayTreeNodeBase() {}
};

namespace ost {

template <class GetNode> class BaseAlgo {

private:
  GetNode mGetNode;

public:

  BaseAlgo(const GetNode &getNode) : mGetNode(getNode) {}

  template <class NodePtr>
  OrderStatisticSplayTreeNodeBase<NodePtr> &get_node(NodePtr a) const {
    assert(a);
    assert(mGetNode(a));
    return *mGetNode(a);
  }

  /// @return number of nodes in subtree a.
  template <class NodePtr> size_t get_size(NodePtr a) const {
    return a ? get_node(a).size : 0;
  }

  /// change size attribute.
  template <class NodePtr> void set_size(NodePtr a, size_t value) const {
    if (a)
      get_node(a).size = value;
  }
}; // class BaseAlgo

template <class Update, class GetNode> class UpdateWithSize {
private:
  Update update;
  BaseAlgo<GetNode> algo;

public:
  template <class NodePtr>
  void operator()(NodePtr parent, NodePtr left, NodePtr right) const {
    assert(parent);
    algo.set_size(parent, algo.get_size(left) + algo.get_size(right) + 1);
    update(parent, left, right);
  }
  UpdateWithSize(const Update &update, const GetNode &getNode)
      : update(update), algo(getNode) {}
};

} // namespace ost

template <class Expand, class Update, class GetNode>
class OrderStatisticSplayTreeBase : public SplayTreeBase<
    Expand, ost::UpdateWithSize<Update, GetNode>, GetNode> {
protected:
  ~OrderStatisticSplayTreeBase() {}

private:
  using NewUpdate = ost::UpdateWithSize<Update, GetNode>;
  using Base = SplayTreeBase<Expand, NewUpdate, GetNode>;
  using Self = OrderStatisticSplayTreeBase;
  GetNode mGetNode;
  ost::BaseAlgo<GetNode> algo;

public:
  using Base::get_left;
  using Base::add;
  using Base::find;
  using Base::splay;

  OrderStatisticSplayTreeBase(const Expand &expand, const Update &update,
                              const GetNode &getNode)
      : Base(expand, NewUpdate(update, getNode), getNode), mGetNode(getNode),
        algo(getNode) {}

  // perfect forward
  template <class NodePtr>
  OrderStatisticSplayTreeNodeBase<NodePtr> &get_node(NodePtr a) const {
    return algo.get_node(a);
  }
  template <class NodePtr> size_t get_size(NodePtr a) const {
    return algo.get_size(a);
  }
  template <class NodePtr> void set_size(NodePtr a, size_t value) const {
    return algo.set_size(a, value);
  }

  template <class NodePtr> size_t get_order(NodePtr p) const {
    splay(p);
    return get_size(get_left(p));
  }

  /// @return the n-th small node.
  template <class NodePtr> NodePtr find_by_order(NodePtr root, size_t n) const {
    assert(n >= 0);
    assert(n < get_size(root));
    return find(root, [&n, this](NodePtr current) {
      NodePtr leftChild = get_left(current);
      size_t size = get_size(leftChild);
      if (size == n)
        return 0;
      else if (size + 1 > n)
        return -1;
      else {
        n -= size + 1;
        return 1;
      }
    });
  }
  /// @return insert a node to the n-th place
  template <class NodePtr>
  NodePtr insert(NodePtr root, size_t n, NodePtr newNode) const {
    assert(n >= 0);
    assert(n <= get_size(root));
    return add(root, newNode, [&n, this](NodePtr current) {
      NodePtr leftChild = get_left(current);
      size_t size = get_size(leftChild);
      if (size == n)
        return -1;
      else if (size + 1 > n)
        return -1;
      else {
        n -= size + 1;
        return 1;
      }
    });
  }
};     // class OrderStatisticSplayTreeBase
}      // namespace detail
}      // namespace sbl
#endif /* end of include guard: ORDER_STATISTIC_SPLAY_TREE_HPP_CBTKNGFR */
