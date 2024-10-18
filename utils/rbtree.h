/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-10
 * @LastEditors: lize
 */

#pragma once
#include <algorithm>
#include <atomic>
#include <cassert>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>
namespace lz {
namespace rbtree {
enum class TreeColor : uint8_t { RED = 0U, BLACK };

template <typename Value, typename Compare = std::less<Value>()>
struct Node {
  using NodeUPtr = std::unique_ptr<Node>;
  using NodeSPtr = std::shared_ptr<Node>;
  Node() = default;
  explicit Node(Value value) : _value(std::move(value)) {
  }
  Node(Value value, TreeColor color) : _value(value), _color(color) {
  }
  NodeSPtr sibling() const {
    if (_parent == nullptr) {
      return nullptr;
    }
    if (this == _parent->_left.get()) {
      return _parent->_right;
    }
    return _parent->_left;
  }

  NodeSPtr closestNephew() const {
    if (_parent == nullptr) {
      return nullptr;
    }
    if (_parent->_left == nullptr || _parent->_right == nullptr) {
      return nullptr;
    }
    if (this == _parent->_left.get()) {
      return _parent->_right->_left;
    }
    return _parent->_left->_right;
  }
  NodeSPtr distantNephew() const {
    if (_parent == nullptr) {
      return nullptr;
    }
    if (_parent->_left == nullptr || _parent->_right == nullptr) {
      return nullptr;
    }
    if (this == _parent->_left.get()) {
      return _parent->_right->_right;
    }
    return _parent->_left->_left;
  }

  NodeSPtr _parent{};
  NodeSPtr _left{};
  NodeSPtr _right{};

  Value _value{};
  TreeColor _color = TreeColor::RED;
};

template <typename Value, typename Compare = std::less<Value>()>
class RBTree {
 public:
  using Node = Node<Value, Compare>;
  using NodeSPtr = std::shared_ptr<Node>;
  using NodeUPtr = std::unique_ptr<Node>;
  // TODO() not Value&& or const Value&. Value deal all condition.
  template <typename T>
    requires std::is_convertible_v<T, Value>
  bool insert(T&& value) {
    std::lock_guard<std::mutex> lock(_mutex);
    // first insert as nomal BST
    auto node = insertValue(_root, std::forward<T>(value));
    if (node == nullptr) {
      return false;
    }
    // then rotate or change color to keep balance
    fixupAfterInsert(node);
    return true;
  };
  void remove(Value value) {
    NodeSPtr node = find(value);
    std::lock_guard<std::mutex> lock(_mutex);
    if (node == nullptr) {
      return;
    }
    if (_count == 1 && _root->_value == value) {
      _root = nullptr;
      --_count;
      return;
    }
    // if node has two child, find the max value in left child
    // and replace node with max value node.
    if (node->_left && node->_right) {
      NodeSPtr lower_node = findRightestNode(node->_left);
      // auto lower_node_parent = lower_node->parent;
      std::swap(node->_value, lower_node->_value);
      node = lower_node;
      // swapNodePtr(lower_node, node);
    };

    // if node has one child, replace node with child. and change color to black
    // not like front case to exchange value. because child is not leaf node.
    if (node->_left || node->_right) {
      auto child = node->_left ? node->_left : node->_right;
      auto parent = node->_parent;

      // edge case
      if (parent == nullptr) [[unlikely]] {
        _root = child;
        _root->_parent = nullptr;
        _root->_color = TreeColor::BLACK;
        --_count;
        return;
      }
      if (parent->_left == node) {
        parent->_left = child;
      } else {
        parent->_right = child;
      }
      child->_parent = parent;
      child->_color = TreeColor::BLACK;
      --_count;
      return;
    }
    // if node is red leaf node. just remove it.
    if (node->_color == TreeColor::RED) {
      if (node->_parent->_left == node) {
        node->_parent->_left = nullptr;
      } else {
        node->_parent->_right = nullptr;
      }
      --_count;
      return;
    }
    // if node is black leaf node
    removeBlackLeafNode(node);
    // edge case
    if (node == _root) [[unlikely]] {
      --_count;
      _root = nullptr;
      return;
    }
    if (node->_parent->_left == node) {
      node->_parent->_left = nullptr;
    } else {
      node->_parent->_right = nullptr;
    }

    --_count;
  }

  NodeSPtr find(Value value) {
    std::lock_guard<std::mutex> lock(_mutex);
    NodeSPtr node = _root;
    while (node) {
      if (value < node->_value) {
        node = node->_left;
      } else if (value > node->_value) {
        node = node->_right;
      } else {
        return node;
      }
    }
    return nullptr;
  };
  NodeSPtr findMin() {
    std::lock_guard<std::mutex> lock(_mutex);
    return findLeftestNode(_root);
  };
  std::size_t size() const {
    return _count;
  }
  NodeSPtr root() const {
    return _root;
  }
  std::pair<bool, int> checkRbTree() {
    auto count = checkEachPath(_root, 0);
    return count == -1 ? std::make_pair(false, count)
                       : std::make_pair(true, count);
  }
  void printTree(NodeSPtr node = nullptr,
                 int level = 0,
                 const std::string& prefix = "Root: ") {
    if (_root == nullptr) {
      return;
    }
    if (node == nullptr) {
      node = _root;
    }

    std::cout << std::setw(level * 4) << prefix << node->_value
              << (node->_color == TreeColor::RED ? "(R)" : "(B)") << std::endl;
    if (node->_left != nullptr || node->_right != nullptr) {
      if (node->_left) {
        printTree(node->_left, level + 1, "L--- ");
      } else {
        std::cout << std::setw((level + 1) * 4) << "L--- " << "None"
                  << std::endl;
      }
      if (node->_right) {
        printTree(node->_right, level + 1, "R--- ");
      } else {
        std::cout << std::setw((level + 1) * 4) << "R--- " << "None"
                  << std::endl;
      }
    }
  }

 private:
  //     [3]                   4
  //  1      4      ==>    3      5
  //       2   5        1    2       6
  //             6
  void rotateLeft(NodeSPtr node) {
    NodeSPtr parent = node->_parent;
    NodeSPtr rightNode = node->_right;
    NodeSPtr successor = node->_right->_left;

    rightNode->_left = node;
    node->_right = successor;

    rightNode->_parent = node->_parent;
    node->_parent = rightNode;

    if (successor) {
      successor->_parent = node;
    }
    if (parent) {
      if (node == parent->_left) {
        parent->_left = rightNode;
      } else {
        parent->_right = rightNode;
      }
    }
    if (node == _root) {
      _root = rightNode;
    }
  }
  //         [5]                    3
  //       3      6     ==>      2      5
  //    2    4                1      4     6
  // 1
  // dual operation of rotateLeft. just swap text "right" with "left"
  void rotateRight(NodeSPtr node) {
    NodeSPtr parent = node->_parent;
    NodeSPtr leftNode = node->_left;
    NodeSPtr successor = node->_left->_right;

    // update node's child
    leftNode->_right = node;
    node->_left = successor;

    // update node's parent
    leftNode->_parent = node->_parent;
    node->_parent = leftNode;

    if (successor) {
      successor->_parent = node;
    }
    if (parent) {
      if (node == parent->_right) {
        parent->_right = leftNode;
      } else {
        parent->_left = leftNode;
      }
    }
    if (node == _root) {
      _root = leftNode;
    }
  }
  //        G(b)                G(r)
  //     P(r)  U(r)   ===>    P(b)  U(b)
  //   N(r)                 N(r)
  void recolor(NodeSPtr node) {
    assert(node != nullptr);

    // recolor(node->_parent->_parent);
  }
  template <typename T>
  NodeSPtr insertValue(NodeSPtr node, T&& value) {
    if (node == nullptr) {
      _root = std::make_shared<Node>(value);
      ++_count;
      return _root;
    }
    while (node) {
      if (value < node->_value) {
        if (node->_left == nullptr) {
          node->_left = std::make_shared<Node>(value);
          node->_left->_parent = node;
          ++_count;
          return node->_left;
        }
        node = node->_left;
      } else if (value > node->_value) {
        if (node->_right == nullptr) {
          node->_right = std::make_shared<Node>(value);
          node->_right->_parent = node;
          ++_count;
          return node->_right;
        }
        node = node->_right;
      } else {
        return nullptr;
      }
    }
    // should not reach here
    return nullptr;
  }
  NodeSPtr findRightestNode(NodeSPtr node) const {
    if (node == nullptr) [[unlikely]] {
      return nullptr;
    }
    while (node->_right) {
      node = node->_right;
    }
    return node;
  }
  NodeSPtr findLeftestNode(NodeSPtr node) const {
    if (node == nullptr) [[unlikely]] {
      return nullptr;
    }
    while (node->_left) {
      node = node->_left;
    }
    return node;
  }
  void removeBlackLeafNode(NodeSPtr node) {
    if (node == _root) {
      return;
    }
    auto parent = node->_parent;
    auto sibling = node->sibling();

    // Case 1: Sibling is RED, parent and nephews must be BLACK
    //   Step 1. If N is a left child, left rotate P;
    //           If N is a right child, right rotate P.
    //   Step 2. Paint S to BLACK, P to RED
    //   Step 3. Goto Case 2, 3, 4, 5
    //      [P]                   <S>               [S]
    //      / \    l-rotate(P)    / \    repaint    / \
    //    [N] <S>  ==========>  [P] [D]  ======>  <P> [D]
    //        / \               / \               /  \
    //      [C] [D]           [N] [C]           [N] [C]
    if (sibling->_color == TreeColor::RED) {
      if (node == parent->_left) {
        rotateLeft(parent);
      } else {
        rotateRight(parent);
      }
      sibling->_color = TreeColor::BLACK;
      parent->_color = TreeColor::RED;
      sibling = node->sibling();
    }

    // Case 2: Sibling and nephews are BLACK, parent is RED
    //   Swap the color of P and S
    //      <P>             [P]
    //      / \             / \
    //    [N] [S]  ====>  [N] <S>
    //        / \             / \
    //      [C] [D]         [C] [D]
    if (parent->_color == TreeColor::RED &&
        sibling->_color == TreeColor::BLACK &&
        (sibling->_left == nullptr ||
         sibling->_left->_color == TreeColor::BLACK) &&
        (sibling->_right == nullptr ||
         sibling->_right->_color == TreeColor::BLACK)) {
      parent->_color = TreeColor::BLACK;
      sibling->_color = TreeColor::RED;
      return;
    }

    // Case 3: Sibling, parent and nephews are all black
    //   Step 1. Paint S to RED
    //   Step 2. Recursively maintain P
    //      [P]             [P]
    //      / \             / \
    //    [N] [S]  ====>  [N] <S>
    //        / \             / \
    //      [C] [D]         [C] [D]

    if (parent->_color == TreeColor::BLACK &&
        sibling->_color == TreeColor::BLACK &&
        (sibling->_left == nullptr ||
         sibling->_left->_color == TreeColor::BLACK) &&
        (sibling->_right == nullptr ||
         sibling->_right->_color == TreeColor::BLACK)) {
      sibling->_color = TreeColor::RED;
      removeBlackLeafNode(parent);
      return;
    }
    // Case 4: Sibling is BLACK, close nephew is RED,
    //         distant nephew is BLACK
    //   Step 1. If N is a left child, right rotate S;
    //           If N is a right child, left rotate S.
    //   Step 2. Swap the color of close nephew and sibling
    //   Step 3. Goto case 5
    //                            {P}                {P}
    //      {P}                   / \                / \
    //      / \    r-rotate(S)  [N] <C>   repaint  [N] [C]
    //    [N] [S]  ==========>        \   ======>        \
    //        / \                     [S]                <S>
    //      <C> [D]                     \                  \
    //                                  [D]                [D]

    auto closeNephew = node->closestNephew();
    auto distantNephew = node->distantNephew();
    if (sibling->_color == TreeColor::BLACK &&
        (closeNephew && closeNephew->_color == TreeColor::RED) &&
        (distantNephew == nullptr ||
         distantNephew->_color == TreeColor::BLACK)) {
      if (node == parent->_left) {
        rotateRight(sibling);
      } else {
        rotateLeft(sibling);
      }
      sibling->_color = TreeColor::RED;
      closeNephew->_color = TreeColor::BLACK;
      sibling = node->sibling();
      distantNephew = node->distantNephew();
      // // update closeNephew and distantNephew
      // if (node == parent->_left) {
      //   closeNephew = sibling->_left;
      //   distantNephew = sibling->_right;
      // } else {
      //   closeNephew = sibling->_right;
      //   distantNephew = sibling->_left;
      // }
    }

    // Case 5: Sibling is BLACK, distant nephew is RED
    //   Step 1. If N is a left child, left rotate P;
    //           If N is a right child, right rotate P.
    //   Step 2. Swap the color of parent and sibling.
    //   Step 3. Paint distant nephew D to BLACK.
    //      {P}                   [S]               {S}
    //      / \    l-rotate(P)    / \    repaint    / \
    //    [N] [S]  ==========>  {P} <D>  ======>  [P] [D]
    //        / \               / \               / \
    //      {C} <D>           [N] {C}           [N] {C}
    if (node == parent->_left) {
      rotateLeft(parent);
    } else {
      rotateRight(parent);
    }
    std::swap(parent->_color, sibling->_color);
    distantNephew->_color = TreeColor::BLACK;
  }
  int checkEachPath(NodeSPtr node, int black_count) {
    if (node == nullptr) {
      black_count++;
      return black_count;
    }
    if (node->_color == TreeColor::BLACK) {
      black_count++;
    }
    // check value
    if ((node->_left && node->_value < node->_left->_value) ||
        (node->_right && node->_value > node->_right->_value)) {
      return -1;
    }
    // check color of node and children
    if (node->_color == TreeColor::RED && node->_left && node->_right &&
        (node->_left->_color == TreeColor::RED &&
         node->_right->_color == TreeColor::RED)) {
      return -1;
    }
    auto leftCount = checkEachPath(node->_left, black_count);
    auto rightCount = checkEachPath(node->_right, black_count);
    // check black count
    return leftCount == rightCount ? leftCount : -1;
  }
  void fixupAfterInsert(NodeSPtr node) {
    // then rotate or change color to keep balance

    // case1 tree is empty. only insert RED node.
    // fine, is ok.
    if (node == _root) {
      return;
    }
    if (node->_parent == _root) {
      // need to change root color to black
      if (_root->_color == TreeColor::RED) [[unlikely]] {
        _root->_color = TreeColor::BLACK;
        return;
      }
      return;
    }

    // case3 only 2 node and  black root node in tree.
    // fine, is ok.
    if (_count == 2 && _root->_color == TreeColor::BLACK) {
      return;
    }

    auto parent = node->_parent;
    assert(parent != nullptr);
    auto grandParent = parent->_parent;
    assert(grandParent != nullptr);

    // if P is black, insert is ok. so wo only need to consider is U.
    if (parent->_color == TreeColor::BLACK) {
      return;
    }

    // if U is not exist
    // if (grandParent->_left == nullptr) {
    //   rotateLeft(grandParent);
    //   grandParent->_color = TreeColor::RED;
    //   parent->_color = TreeColor::BLACK;
    //   return;
    // } else if (grandParent->_right == nullptr) {
    //   rotateRight(grandParent);
    //   grandParent->_color = TreeColor::RED;
    //   parent->_color = TreeColor::BLACK;
    //   return;
    // }

    // case4 if U is red.
    //      G(b)              G(r)
    //   P(r)  U(r)  ===>  P(b)  U(b)
    // N(r)               N(r)
    // need change color of G, P, U.  G'parent is unknow ,so recursively recolor
    if (grandParent->_left && grandParent->_right &&
        grandParent->_left->_color == TreeColor::RED &&
        grandParent->_right->_color == TreeColor::RED) {
      // recolor(node);
      grandParent->_color = TreeColor::RED;
      grandParent->_left->_color = TreeColor::BLACK;
      grandParent->_right->_color = TreeColor::BLACK;
      fixupAfterInsert(grandParent);
      return;
    }
    // if U is balck, according to the order of node, parent, grandParent
    // LL RR LR RL

    // case5 LR && RL.
    //      G(b)   L-rotate     G(b)
    //   P(r)  U(b)  ===>    P(r)  U(b)
    //     N(r)           N(r)
    // need rotate to trans case6
    if (node == parent->_right && parent == grandParent->_left) {
      rotateLeft(parent);
      // update node and parent for case6
      std::swap(parent, node);
    } else if (node == parent->_left && parent == grandParent->_right) {
      rotateRight(parent);
      std::swap(parent, node);
    }

    // case6 LL && RR.  RR is dual operation of LL
    //      G(b)   R-rotate   P(r)      recolor   P(b)
    //   P(r)  U(b)  ===>  N(r)  G(b)     ===>  N(r)  G(r)
    // N(r)                        U(b)                  U(b)
    if (node == parent->_left && parent == grandParent->_left) {
      rotateRight(grandParent);
    } else if (node == parent->_right && parent == grandParent->_right) {
      rotateLeft(grandParent);
    }
    grandParent->_color = TreeColor::RED;
    parent->_color = TreeColor::BLACK;
  }
  void swapNodePtr(NodeSPtr& lhs, NodeSPtr& rhs) {
    std::swap(lhs->_value, rhs->_value);
    std::swap(lhs, rhs);
  }

 private:
  NodeSPtr _root{};
  std::atomic<size_t> _count = 0;
  std::mutex _mutex{};
};
}  // namespace rbtree
}  // namespace lz