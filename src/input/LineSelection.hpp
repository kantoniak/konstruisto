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

  [[nodiscard]] glm::ivec2 getFrom() const override;
  [[nodiscard]] glm::ivec2 getTo() const override;

  [[nodiscard]] const std::vector<data::Position> getSelected() const override;
  [[nodiscard]] const std::vector<LineSelection> divideByChunk() const;

private:
  unsigned short lineWidth;

  [[nodiscard]] unsigned int getLength() const;
};
}

#endif
