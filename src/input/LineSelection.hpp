#ifndef INPUT_LINESELECTION_HPP
#define INPUT_LINESELECTION_HPP

#include <vector>

#include "../data/Position.hpp"
#include "Selection.hpp"

namespace input {

class LineSelection : public Selection {

public:
  LineSelection(unsigned short lineWidth);

  virtual glm::ivec2 getFrom() const;
  virtual glm::ivec2 getTo() const;

  virtual const std::vector<data::Position> getSelected() const;

private:
  unsigned short lineWidth;
};
}

#endif
