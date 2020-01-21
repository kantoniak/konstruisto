#ifndef DATA_TREEGROUP_HPP
#define DATA_TREEGROUP_HPP

#include <algorithm>
#include <array>
#include <list>
#include <memory>

#include "Tree.hpp"

namespace data {

class TreeGroup {
public:
  TreeGroup() noexcept;

  void add_tree(data::Tree::ptr tree) noexcept;
  bool remove_tree(const Tree& tree) noexcept;
  void update(float delta_in_turns) noexcept;

  [[nodiscard]] size_t get_count(Tree::Type type) const noexcept;
  [[nodiscard]] const std::list<Tree::ptr>& get_trees(Tree::Type type) const noexcept;

private:
  std::array<uint32_t, Tree::TREE_TYPE_COUNT> type_to_count;
  std::array<std::list<Tree::ptr>, Tree::TREE_TYPE_COUNT> trees;
};
}

#endif