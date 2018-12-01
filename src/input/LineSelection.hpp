#ifndef INPUT_LINESELECTION_HPP
#define INPUT_LINESELECTION_HPP

#include <vector>

#include "../data/Chunk.hpp"
#include "../data/Position.hpp"
#include "Selection.hpp"

namespace input {

class LineSelection : public Selection {

public:
  explicit LineSelection(unsigned short lineWidth);

  glm::ivec2 getFrom() const override;
  glm::ivec2 getTo() const override;

  const std::vector<data::Position> getSelected() const override;
  const std::vector<LineSelection> divideByChunk() const;

private:
  unsigned short lineWidth;

  unsigned int getLength() const;
};
}

#endif
