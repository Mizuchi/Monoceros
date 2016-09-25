#ifndef CHAIN_HPP_OZMNRWAJ
#define CHAIN_HPP_OZMNRWAJ

#include "vector_tree.hpp"
#include "binary_tree.hpp"
#include "detail/order_statistic_splay_tree.hpp"

namespace sbl {
namespace chain {

template <class NodeTraits> class Replace {

private:
  using T = typename NodeTraits::value_type;
  using Node = typename NodeTraits::TNode;
  Node &node() { return static_cast<Node &>(*this); }
  bool isReplaced;

public:
  Replace(const T &) : isReplaced(false) {
    static_assert(std::is_base_of<Replace, Node>::value, "");
    static_assert(Node::template Has< ::sbl::chain::Replace>::value, "");
  }
  template <class T> void expand(T *, Node *, Node *) {}
  template <class T> void update(T *, Node *, Node *) {}
  void expand(Node *, Node *left, Node *right) {
    assert(this);
    if (isReplaced) {
      isReplaced = false;
      if (left)
        left->set_replaced(node().value);
      if (right)
        right->set_replaced(node().value);
    }
  }
  bool is_replaced() const { return isReplaced; }
  void set_replaced(const T &value) {
    if (this) {
      static_assert(std::is_base_of<Replace, Node>::value, "");
      isReplaced = true;
      node().value = value;
      node().update(this);
    }
  }
};

template <class NodeTraits> class Reverse {
private:
  using T = typename NodeTraits::value_type;
  using Node = typename NodeTraits::TNode;
  Node &node() { return static_cast<Node &>(*this); }
  bool isReversed;

public:
  Reverse(const T &) : isReversed(false) {
    static_assert(std::is_base_of<Reverse, Node>::value, "");
    static_assert(Node::template Has< ::sbl::chain::Reverse>::value, "");
  }
  template <class T> void expand(T *, Node *, Node *) {}
  template <class T> void update(T *, Node *, Node *) {}
  void expand(Node *, Node *left, Node *right) {
    assert(this);
    if (isReversed) {
      isReversed = false;
      make_binary_tree().swap_child(&node());
      if (left)
        left->set_reversed();
      if (right)
        right->set_reversed();
    }
  }
  void set_reversed() {
    if (this) {
      isReversed = not isReversed;
      node().update(this);
    }
  }
};

template <class NodeTraits> class Sum {
private:
  using T = typename NodeTraits::value_type;
  using Node = typename NodeTraits::TNode;
  Node &node() { return static_cast<Node &>(*this); }
  T sum;

public:
  template <class T> void expand(T *, Node *, Node *) {}
  template <class T> void update(T *, Node *, Node *) {}
  const T &get() const { return sum; }
  Sum(const T &value) : sum(value) {
    static_assert(std::is_base_of<Sum, Node>::value, "");
    static_assert(Node::template Has< ::sbl::chain::Sum>::value, "");
  }
  void expand(Node *, Node *, Node *) {}
  void update(Node *, Node *left, Node *right) {
    assert(this);
    sum = node().value;
    for (Node *p : { left, right }) {
      if (p)
        sum += static_cast<Sum &>(*p).sum;
    }
  }
  void update(Replace<NodeTraits> *, Node *, Node *) {
    auto tree = detail::make_tree<detail::ost::BaseAlgo>([](Node *p) {
      return static_cast<VectorTreeNode<Node *> *>(p);
    });
    sum = node().value * tree.get_size(&node());
  }
};

template <class Less = Less> struct Compare {
  template <class NodeTraits> class Top {
  private:
    using T = typename NodeTraits::value_type;
    using Node = typename NodeTraits::TNode;
    Node &node() { return static_cast<Node &>(*this); }
    Less compare;
    T top;
    void test(const T &v) {
      if (compare(top, v))
        top = v;
    }

  public:
    const T &get() const { return top; }
    Top(const T &value) : top(value) {
      static_assert(std::is_base_of<Top, Node>::value, "");
      static_assert(Node::template Has<Compare::Top>::value, "");
    }
    template <class T> void expand(T *, Node *, Node *) {}
    template <class T> void update(T *, Node *, Node *) {}
    void update(Node *, Node *left, Node *right) {
      assert(this);
      top = node().value;
      for (Node *p : { left, right }) {
        if (p)
          test(static_cast<Top &>(*p).top);
      }
    }
    void update(Replace<NodeTraits> *, Node *, Node *) { top = node().value; }
  }; // class Top

  template <class NodeTraits> class MaxElement {
  private:
    using T = typename NodeTraits::value_type;
    using Node = typename NodeTraits::TNode;
    Node &node() { return static_cast<Node &>(*this); }
    Less compare;
    Node *pMaxElement;
    void test(Node *p) {
      if (compare(pMaxElement->value, p->value))
        pMaxElement = p;
    }

  public:
    template <class T> void expand(T *, Node *, Node *) {}
    template <class T> void update(T *, Node *, Node *) {}
    Node *get() const { return pMaxElement; }
    MaxElement(const T &) : pMaxElement(&node()) {
      static_assert(std::is_base_of<MaxElement, Node>::value, "");
      static_assert(Node::template Has<Compare::MaxElement>::value, "");
    }
    void update(Node *, Node *left, Node *right) {
      assert(this);
      pMaxElement = &node();
      for (Node *p : { left, right }) {
        if (p)
          test(static_cast<MaxElement &>(*p).pMaxElement);
      }
    }
    void update(Replace<NodeTraits> *, Node *, Node *) {
      pMaxElement = &node();
    }
  }; // class MaxElement

  template <class NodeTraits> class MaxSum {
  private:
    using T = typename NodeTraits::value_type;
    using Node = typename NodeTraits::TNode;
    Less compare;
    Node &node() { return static_cast<Node &>(*this); }

  public:
    T leftMaxSum, rightMaxSum, maxSum;
    MaxSum(const T &value)
        : leftMaxSum(value), rightMaxSum(value), maxSum(value) {
      static_assert(std::is_base_of<MaxSum, Node>::value, "");
      static_assert(std::is_base_of<Sum<NodeTraits>, Node>::value, "");
    }
    const T &get() const { return maxSum; }
    template <class T> void expand(T *, Node *, Node *) {}
    template <class T> void update(T *, Node *, Node *) {}

  private:
    void update_max(T &v, const T &u) { v = std::max(v, u, compare); }
    void max_sum(MaxSum *left, MaxSum *right) {
      const T &elem = node().value;
      maxSum = elem;
      if (left) {
        update_max(maxSum, left->maxSum);
        update_max(maxSum, left->rightMaxSum + elem);
      }
      if (right) {
        update_max(maxSum, right->maxSum);
        update_max(maxSum, right->leftMaxSum + elem);
      }
      if (left and right)
        update_max(maxSum, left->rightMaxSum + elem + right->leftMaxSum);
    }

    void max_left_sum(MaxSum *left, MaxSum *right) {
      const T &elem = node().value;
      leftMaxSum = elem;
      if (left) {
        const T &leftAllSum =
            static_cast<Sum<NodeTraits> *>(static_cast<Node *>(left))->get();
        leftMaxSum = left->leftMaxSum;
        update_max(leftMaxSum, leftAllSum + elem);
        if (right)
          update_max(leftMaxSum, leftAllSum + elem + right->leftMaxSum);
      } else if (right)
        update_max(leftMaxSum, elem + right->leftMaxSum);
    }
    void max_right_sum(MaxSum *left, MaxSum *right) {
      const T &elem = node().value;
      rightMaxSum = elem;
      if (right) {
        const T &rightAllSum =
            static_cast<Sum<NodeTraits> *>(static_cast<Node *>(right))->get();
        rightMaxSum = right->rightMaxSum;
        update_max(rightMaxSum, elem + rightAllSum);
        if (left)
          update_max(rightMaxSum, left->rightMaxSum + elem + rightAllSum);
      } else if (left)
        update_max(rightMaxSum, elem + left->rightMaxSum);
    }

  public:
    void update(Node *, Node *left, Node *right) {
      MaxSum *pLeft = left ? static_cast<MaxSum *>(left) : nullptr;
      MaxSum *pRight = right ? static_cast<MaxSum *>(right) : nullptr;
      max_sum(pLeft, pRight);
      max_left_sum(pLeft, pRight);
      max_right_sum(pLeft, pRight);
    }
    void update(Replace<NodeTraits> *, Node *, Node *) {
      if (node().value > 0) {
        auto tree = detail::make_tree<detail::ost::BaseAlgo>([](Node *p) {
          return static_cast<VectorTreeNode<Node *> *>(p);
        });
        maxSum = leftMaxSum = rightMaxSum =
            node().value * tree.get_size(&node());
      } else {
        maxSum = leftMaxSum = rightMaxSum = node().value;
      }
    }
    void update(Reverse<NodeTraits> *, Node *, Node *) {
      std::swap(leftMaxSum, rightMaxSum);
    }
  }; // template<class NodeTraits> class MaxSum

}; // template <class Less> class Compare

struct HasFeature {
  template <template <class> class T, template <class> class... Features>
  struct Has;

  template <template <class> class T, template <class> class... Features>
  struct Has<T, T, Features...> : std::true_type {};

  template <template <class> class T, template <class> class Feature,
            template <class> class... Features>
  struct Has<T, Feature, Features...> : Has<T, Features...> {};

  template <template <class> class T> struct Has<T> : std::false_type {};
}; // struct HasFeature

template <class T, template <class> class... Features> struct Node;

template <class T, template <class> class... Features> struct NodeTraits {
  using value_type = T;
  template <template <class> class U>
  using Has = HasFeature::Has<U, Features...>;
  using TNode = Node<T, Features...>;
  using Traits = NodeTraits<T, Features...>;
};

template <class T, template <class> class... Features>
struct Node : VectorTreeNode<Node<T, Features...> *>,
              Features<NodeTraits<T, Features...> >... {
public:
  using value_type = T;
  template <template <class> class U>
  using Has = HasFeature::Has<U, Features...>;
  using TNode = Node<T, Features...>;
  using Traits = NodeTraits<T, Features...>;

public:
  T value;
  Node(const T &v = T()) : Features<Traits>(v)..., value(v) {}

private:
  template <class..., class Parent>
  void run_expand(Parent *, std::false_type) {}
  template <class Base, class... Bases, class Parent>
  void run_expand(Parent *parent, std::true_type) {
    static_assert(std::is_base_of<Base, Node>::value, "");
    auto tree = make_binary_tree();
    Base::expand(parent, tree.get_left(this), tree.get_right(this));
    run_expand<Bases...>(parent);
  }
  template <class... Bases, class Parent> void run_expand(Parent *parent) {
    run_expand<Bases...>(
        parent, std::integral_constant<bool, bool(sizeof...(Bases))>());
  }

public:
  template <class Parent> void expand(Parent *parent) {
    run_expand<Features<Traits>...>(parent);
  }

private:
  template <class..., class Parent>
  void run_update(Parent *, std::false_type) {}
  template <class Base, class... Bases, class Parent>
  void run_update(Parent *parent, std::true_type) {
    static_assert(std::is_base_of<Base, Node>::value, "");
    auto tree = make_binary_tree();
    Base::update(parent, tree.get_left(this), tree.get_right(this));
    run_update<Bases...>(parent);
  }
  template <class... Bases, class Parent> void run_update(Parent *parent) {
    run_update<Bases...>(
        parent, std::integral_constant<bool, bool(sizeof...(Bases))>());
  }

public:
  template <class Parent> void update(Parent *parent) {
    run_update<Features<Traits>...>(parent);
  }
};

template <class Tree> class Chain {
private:
  Tree tree;
  using Node = typename std::remove_pointer<typename Tree::NodePtr>::type;
  using T = typename Node::value_type;

  size_t get_order(Node *s) const { return tree.get_order(s); }

public:
  template <template <class> class Feature>
  using Has = typename Node::template Has<Feature>;
  Chain(const Tree &tree) : tree(tree) {}
  using value_type = T;

  const T &operator[](size_t idx) const {
    return tree.find_by_order(idx)->value;
  }
  T &operator[](size_t idx) { return tree.find_by_order(idx)->value; }

public:
  // metadata
  size_t size() const { return tree.size(); }

public:
  // base
  void insert(size_t idx, const T &v) { tree.insert(idx, new Node(v)); }
  void erase(size_t left, size_t right);

public:
  // modify
  void replace(size_t left, size_t right, const T &value);
  void reverse(size_t left, size_t right);

public:
  // query
  T sum(size_t left, size_t right) const;

  template <class Less = Less> T max_sum(size_t left, size_t right) const;
  template <class Less = Less> T top(size_t left, size_t right) const;
  template <class Less = Less>
  size_t max_element(size_t left, size_t right) const;
  size_t lcp(size_t first, size_t second) const;

  void print(Node *node, size_t depth) const {
    auto b = make_binary_tree();
    if (node) {
      node->expand(node);
      print(b.get_left(node), depth + 1);
      for (size_t i = 0; i < depth; i++)
        cout << "    ";
      cout << node->value << endl;
      print(b.get_right(node), depth + 1);
    }
  }

  void print() const { print(tree.root, 0); }

  template <template <class> class U> constexpr static auto has() {
    return Has<U>();
  }
}; // struct Tree

template <class Tree> void Chain<Tree>::erase(size_t left, size_t right) {
  assert(left <= right);
  auto b = make_binary_tree();
  b.postorder_traversal(tree.erase(left, right),
                        [](Node *p, size_t) { delete p; });
}

template <class Tree>
void Chain<Tree>::replace(size_t left, size_t right, const T &value) {
  assert(left <= right);
  tree.call_segment(left, right, [&value](auto *p) { p->set_replaced(value); });
}

template <class Tree> void Chain<Tree>::reverse(size_t left, size_t right) {
  assert(left <= right);
  tree.call_segment(left, right, [](auto *p) { p->set_reversed(); });
}

template <class Tree>
template <class Less>
typename Chain<Tree>::value_type Chain<Tree>::top(size_t left,
                                                  size_t right) const {
  assert(left <= right);
  using TopNode = typename Compare<Less>::template Top<typename Node::Traits>;
  static_assert(std::is_base_of<TopNode, Node>::value, "");
  if (left == right)
    return T();
  TopNode *p;
  tree.call_segment(left, right,
                    [&p](Node *pNode) { p = static_cast<TopNode *>(pNode); });
  return p->get();
}

template <class Tree>
template <class Less>
size_t Chain<Tree>::max_element(size_t left, size_t right) const {
  assert(left <= right);
  using MaxElementNode =
      typename Compare<Less>::template MaxElement<typename Node::Traits>;
  static_assert(std::is_base_of<MaxElementNode, Node>::value, "");
  if (left == right)
    return T();
  Node *p;
  tree.call_segment(left, right, [&p](Node *pNode) {
    p = static_cast<MaxElementNode *>(pNode)->get();
  });
  return get_order(p);
}

template <class Tree>
typename Chain<Tree>::value_type Chain<Tree>::sum(size_t left,
                                                  size_t right) const {
  assert(left <= right);
  if (left == right)
    return T();
  Sum<typename Node::Traits> *p;
  tree.call_segment(left, right, [&p](Node *pNode) { p = pNode; });
  return p->get();
}

template <class Tree>
template <class Less>
typename Chain<Tree>::value_type Chain<Tree>::max_sum(size_t left,
                                                      size_t right) const {
  assert(left <= right);
  using MaxSumNode =
      typename Compare<Less>::template MaxSum<typename Node::Traits>;
  static_assert(std::is_base_of<MaxSumNode, Node>::value, "");
  if (left == right)
    return T();
  MaxSumNode *p;
  tree.call_segment(
      left, right, [&p](Node *pNode) { p = static_cast<MaxSumNode *>(pNode); });
  return p->get();
}

template <class T, template <class> class... Features> auto make_chain() {
  using NodePtr = Node<T, Features...> *;
  auto tree = make_vector_tree([](NodePtr parent, NodePtr,
                                  NodePtr) { parent->expand(parent); },
                               [](NodePtr parent, NodePtr,
                                  NodePtr) { parent->update(parent); });
  return Chain<decltype(tree)>(tree);
}

template <class Tree, class T>
Chain<Tree> &operator+=(Chain<Tree> &chain, std::initializer_list<T> l) {
  for (const T &v : l)
    chain.insert(chain.size(), v);
  return chain;
}
} // namespace chain
using chain::make_chain;
using chain::Replace;
using chain::Reverse;
using chain::Compare;
using chain::Sum;
} // namespace sbl
#endif /* end of include guard: CHAIN_HPP_OZMNRWAJ */
