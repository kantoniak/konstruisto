#include "LineSelection.hpp"

#include <iostream>

namespace input {
LineSelection::LineSelection(unsigned short lineWidth) : lineWidth(lineWidth) {
}

glm::ivec2 LineSelection::getFrom() const {
  const glm::ivec2 diff = toPoint - fromPoint;
  const bool goesNorth = std::abs(diff.y) > std::abs(diff.x);

  if (goesNorth) {
    return glm::ivec2(fromPoint.x, std::min(toPoint.y, fromPoint.y));
  } else {
    return glm::ivec2(std::min(toPoint.x, fromPoint.x), fromPoint.y);
  }

  return Selection::getFrom();
}

glm::ivec2 LineSelection::getTo() const {
  const glm::ivec2 diff = toPoint - fromPoint;
  const bool goesNorth = std::abs(diff.y) > std::abs(diff.x);

  if (goesNorth) {
    return glm::ivec2(fromPoint.x + lineWidth - 1, std::max(toPoint.y, fromPoint.y + lineWidth - 1));
  } else {
    return glm::ivec2(std::max(toPoint.x, fromPoint.x + lineWidth - 1), fromPoint.y + lineWidth - 1);
  }
}
}