#ifndef SPLAY_TREE_HPP_WHMEINAS
#define SPLAY_TREE_HPP_WHMEINAS
#include"detail/splay_tree.hpp"

namespace sbl {

template <class NodePtr>
using SplayTreeNode = Derived<detail::SplayTreeNodeBase<NodePtr> >;

template <class Expand = detail::DoNothing, class Update = detail::DoNothing,
          class GetNode = detail::GetNodeDefault<detail::SplayTreeNodeBase> >
auto make_splay_tree(const Expand &expand = Expand(),
                   const Update &update = Update(),
                   const GetNode &getNode = GetNode()) {
  return detail::make_tree<detail::SplayTreeBase>(expand, update, getNode);
}

}      // namespace sbl
#endif /* end of include guard: SPLAY_TREE_HPP_WHMEINAS */
