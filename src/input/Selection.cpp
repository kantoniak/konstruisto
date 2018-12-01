#include "Selection.hpp"

namespace input {
Selection::Selection() : selecting(false), valid(true) {
  reset();
  startColor = glm::vec4();
  selectionColor = glm::vec4();
  invalidColor = glm::vec4();
}

Selection& Selection::operator=(const Selection& other) = default;

void Selection::setColors(glm::vec4 startColor, glm::vec4 selectionColor, glm::vec4 invalidColor) {
  this->startColor = startColor;
  this->selectionColor = selectionColor;
  this->invalidColor = invalidColor;
}

glm::vec4 Selection::getColor() const {
  if (isSelecting() && isValid()) {
    return selectionColor;
  }
  if (isSelecting() && !isValid()) {
    return invalidColor;
  }
  return startColor;
}

void Selection::start(glm::ivec2 point) {
  fromPoint = point;
  toPoint = point;
  selecting = true;
}

void Selection::from(glm::ivec2 point) {
  fromPoint = point;
}

void Selection::to(glm::ivec2 point) {
  toPoint = point;
}

void Selection::stop() {
  selecting = false;
}

void Selection::markInvalid() {
  valid = false;
}

void Selection::markValid() {
  valid = true;
}

bool Selection::isValid() const {
  return valid;
}

bool Selection::isSelecting() const {
  return selecting;
}

void Selection::reset() {
  selecting = false;
  valid = true;
  fromPoint = toPoint = glm::ivec2();
}

glm::ivec2 Selection::getFrom() const {
  glm::ivec2 result = fromPoint;
  if (fromPoint.x > toPoint.x) {
    result.x = toPoint.x;
  }
  if (fromPoint.y > toPoint.y) {
    result.y = toPoint.y;
  }
  return result;
}

glm::ivec2 Selection::getTo() const {
  glm::ivec2 result = toPoint;
  if (fromPoint.x > toPoint.x) {
    result.x = fromPoint.x;
  }
  if (fromPoint.y > toPoint.y) {
    result.y = fromPoint.y;
  }
  return result;
}

const std::vector<data::Position> Selection::getSelected() const {
  // FIXME(kantoniak): Return all in selection
  return std::vector<data::Position>();
}
}