#ifndef INPUT_LINESELECTION_HPP
#define INPUT_LINESELECTION_HPP

#include <cmath>

#include "Selection.hpp"

namespace input {

class LineSelection : public Selection {

public:
  LineSelection(unsigned short lineWidth);

  virtual glm::ivec2 getFrom() const;
  virtual glm::ivec2 getTo() const;

private:
  unsigned short lineWidth;
};
}

#endif
