#ifndef VECTOR_TREE_HPP_IGHLXQ4M
#define VECTOR_TREE_HPP_IGHLXQ4M
#include"function_traits.hpp"
#include"detail/vector_tree.hpp"

namespace sbl {
namespace detail {
/// @brief a low-level vector like data structure, used to implement sbl::Chain.
template <class TNodePtr, class Tree> class VectorTree {
public:
  using NodePtr = TNodePtr;
  Tree tree;
  mutable NodePtr root;

  explicit VectorTree(const Tree &tree) : tree(tree), root() {}

  /// @return n-th element
  NodePtr find_by_order(size_t n) const {
    root = tree.find_by_order(root, n);
    return root;
  }

  size_t get_order(NodePtr p) const { return tree.get_order(root = p); }

  /// insert a new node to position n
  void insert(size_t n, NodePtr newNode) {
    root = tree.insert(root, n, newNode);
  }

  /// insert new nodes to position n
  void insert(size_t n, NodePtr beg, NodePtr end) {
    if (beg == end)
      return;
    NodePtr p = end;
    do {
      --p;
      insert(n, p);
    } while (p != beg);
  }

  /// @return number of element
  size_t size() const { return tree.get_size(root); }

  /// replace a segment [l, r) to a given node
  NodePtr replace_segment(size_t l, size_t r, NodePtr newNode) {
    NodePtr oldNode;
    root = tree.replace_segment(root, l, r, newNode, oldNode);
    return oldNode;
  }

  /// call(root of subtree in range [l, r))
  template <class Callable>
  void call_segment(size_t l, size_t r, Callable call) const {
    assert(l < r);
    root = tree.call_segment(root, l, r, call);
  }

  /// remove add element in range [l, r)
  NodePtr erase(size_t l, size_t r) {
    if (l == r)
      return nullptr;
    return replace_segment(l, r, nullptr);
  }
}; // class VectorTree

template <class NodePtr>
using VectorTreeNode = Derived<VectorTreeNodeBase<NodePtr> >;

template <class NodePtr, class Expand = DoNothing,
          class Update = DoNothing,
          class GetNode = GetNodeDefault<VectorTreeNodeBase> >
auto make_vector_tree(const Expand &expand = Expand(),
                    const Update &update = Update(),
                    const GetNode &getNode = GetNode()) {
  auto tree =
      make_tree<VectorSplayTreeBase>(expand, update, getNode);
  return VectorTree<NodePtr, decltype(tree)>(tree);
}

template <class Expand = DoNothing, class Update = DoNothing,
          class GetNode = GetNodeDefault<VectorTreeNodeBase> >
auto make_vector_tree(const Expand &expand = Expand(),
                    const Update &update = Update(),
                    const GetNode &getNode = GetNode()) {
  using NodePtr = typename function_traits<Expand>::template arg<0>::type;
  return make_vector_tree<NodePtr>(expand, update, getNode);
}
}  //   namespace detail
using detail::make_vector_tree;
using detail::VectorTreeNode;
}      // namespace sbl;
#endif /* end of include guard: VECTOR_TREE_HPP_IGHLXQ4M */
