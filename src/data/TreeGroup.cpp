#include "TreeGroup.hpp"

#include <memory>

namespace data {
TreeGroup::TreeGroup() noexcept {
  for (Tree::Type type : Tree::TYPES) {
    type_to_count[type] = 0;
    trees[type] = std::make_unique<std::vector<Tree>>();
  }
}

void TreeGroup::add_tree(const data::Tree& tree) noexcept {
  trees[tree.get_type()]->push_back(tree);
  type_to_count[tree.get_type()]++;
}

bool TreeGroup::remove_tree(const Tree& tree) noexcept {
  std::vector<Tree>& trees_of_type = *(trees[tree.get_type()]);
  const auto& to_remove = std::find(trees_of_type.begin(), trees_of_type.end(), tree);

  if (to_remove == trees_of_type.end()) {
    return false;
  }

  trees_of_type.erase(to_remove);
  type_to_count[tree.get_type()]--;
  return true;
}

void TreeGroup::update(float delta_in_turns) noexcept {
  for (auto& trees_of_type_ptr : trees) {
    for (auto& tree : *(trees_of_type_ptr)) {
      tree.add_age(delta_in_turns);
    }
  }
}

[[nodiscard]] size_t TreeGroup::get_count(Tree::Type type) const noexcept {
  return type_to_count[type];
}

[[nodiscard]] const std::vector<Tree>& TreeGroup::get_trees(Tree::Type type) const noexcept {
  return *(trees[type]);
}
}