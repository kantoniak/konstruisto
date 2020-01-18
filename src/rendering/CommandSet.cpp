#include "CommandSet.hpp"

namespace rendering {

template <typename Key>
CommandSet<Key>::CommandSet() : contents(new std::vector<CommandSet<Key>::Element>()) {
}

template <typename Key>
bool CommandSet<Key>::push(const CommandSet<Key>::Element element) noexcept {
  if (this->locked) {
    return false;
  }
  this->contents.get()->push_back(std::move(element));
  return true;
}

template <typename Key>
const typename CommandSet<Key>::Element& CommandSet<Key>::next() const noexcept {
  return this->contents.get()->back();
}

template <typename Key>
void CommandSet<Key>::pop() const noexcept {
  return this->contents.get()->pop_back();
}

template <typename Key>
bool CommandSet<Key>::empty() const noexcept {
  return this->contents.get()->empty();
}

template <typename Key>
void CommandSet<Key>::sort() noexcept {
  std::vector<Element>& elems = *(this->contents.get());
  std::sort(elems.begin(), elems.end());
  std::reverse(elems.begin(), elems.end());
  this->locked = true;
}

template <typename Key>
void CommandSet<Key>::clear() noexcept {
  this->contents.get()->clear();
  this->locked = false;
}

template class CommandSet<int32_t>;
}