#ifndef RENDERING_COMMANDSET_HPP
#define RENDERING_COMMANDSET_HPP

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <vector>

#include "Command.hpp"

namespace rendering {

template <typename Key>
class CommandSet {

public:
  using Element = struct Element {
    Command::Type type;
    Key key;
    void* data;

    bool operator<(const struct Element& other) const {
      return key < other.key;
    }
  };

  CommandSet();

  bool push(const Element element) noexcept;
  const Element& next() const noexcept;
  void pop() const noexcept;
  [[nodiscard]] bool empty() const noexcept;

  void sort() noexcept;
  void clear() noexcept;

protected:
  bool locked = false;
  std::unique_ptr<std::vector<Element>> contents;
};
}

#endif
