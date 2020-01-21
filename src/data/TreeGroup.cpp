#include "TreeGroup.hpp"

#include <memory>

namespace data {
TreeGroup::TreeGroup() noexcept {
  for (Tree::Type type : Tree::TYPES) {
    type_to_count[type] = 0;
  }
}

void TreeGroup::add_tree(Tree::ptr tree) noexcept {
  const auto type = (*tree).get_type();
  trees[type].push_back(tree);
  type_to_count[type]++;
}

bool TreeGroup::remove_tree(const Tree& tree) noexcept {
  std::list<Tree::ptr>& trees_of_type = trees[tree.get_type()];
  const auto& to_remove =
      std::find_if(trees_of_type.begin(), trees_of_type.end(), [&](const Tree::ptr& ptr) { return *ptr == tree; });

  if (to_remove == trees_of_type.end()) {
    return false;
  }

  type_to_count[tree.get_type()]--;
  trees_of_type.erase(to_remove);
  return true;
}

void TreeGroup::update(float delta_in_turns) noexcept {
  for (auto& trees_of_type_ptr : trees) {
    for (auto& tree_ptr : trees_of_type_ptr) {
      tree_ptr->add_age(delta_in_turns);
    }
  }
}

size_t TreeGroup::get_count(Tree::Type type) const noexcept {
  return type_to_count[type];
}

const std::list<Tree::ptr>& TreeGroup::get_trees(Tree::Type type) const noexcept {
  return trees[type];
}
}