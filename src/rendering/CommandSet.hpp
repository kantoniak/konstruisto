#ifndef RENDERING_COMMANDSET_HPP
#define RENDERING_COMMANDSET_HPP

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <vector>

namespace rendering {

enum CommandType { DRAW_SINGLE };

template <typename Key>
class CommandSet {

public:
  typedef struct Element {
    CommandType type;
    Key key;
    void* data;

    bool operator<(const struct Element& other) const {
      return key < other.key;
    }
  } Element;

  CommandSet();

  void hint_push_count() noexcept;
  bool push(const Element element) noexcept;
  const Element& next() const noexcept;
  void pop() const noexcept;
  bool empty() const noexcept;

  void sort() noexcept;
  void clear() noexcept;

protected:
  bool locked = false;
  std::unique_ptr<std::vector<Element>> contents;
};
}

#endif
