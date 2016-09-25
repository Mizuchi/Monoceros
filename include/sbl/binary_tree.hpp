#ifndef BINARY_TREE_HPP_KEIC2V5X
#define BINARY_TREE_HPP_KEIC2V5X
#include "detail/binary_tree.hpp"

namespace sbl {
template <class GetNode = detail::GetNodeDefault<detail::BinaryTreeNodeBase> >
auto make_binary_tree(const GetNode &getNode = GetNode()) {
  return detail::make_tree<detail::BinaryTreeBase>(getNode);
}

template <class NodePtr>
using BinaryTreeNode = Derived<detail::BinaryTreeNodeBase<NodePtr> >;
}      // namespace sbl
#endif /* end of include guard: BINARY_TREE_HPP_KEIC2V5X */
