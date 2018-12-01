#include "LineSelection.hpp"

#include <algorithm>

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

const std::vector<data::Position> LineSelection::getSelected() const {
  const glm::ivec2 start = this->getFrom();

  const glm::ivec2 diff = toPoint - fromPoint;
  const bool goesNorth = std::abs(diff.y) > std::abs(diff.x);

  int length = 1 + (goesNorth ? std::abs(diff.y) : std::abs(diff.x));

  std::vector<data::Position> result;
  for (int i = 0; i < length; i++) {
    const glm::ivec2 toAdd = goesNorth ? glm::ivec2(0, i) : glm::ivec2(i, 0);
    result.emplace_back(start + toAdd);
  }

  return result;
}

const std::vector<LineSelection> LineSelection::divideByChunk() const {
  std::vector<LineSelection> result;

  const data::Position from(this->getFrom());
  const data::Position to(this->getTo());

  if (from.getChunk().x != to.getChunk().x) {
    glm::ivec2 sameChunkEnd = to.getGlobal();
    sameChunkEnd.x = from.getGlobal().x - from.getLocal().x + (data::Chunk::SIDE_LENGTH - 1);

    LineSelection sameChunk(lineWidth);
    sameChunk.from(from.getGlobal());
    sameChunk.to(sameChunkEnd);
    result.push_back(sameChunk);

    glm::ivec2 nextChunkStart = sameChunkEnd;
    nextChunkStart.x += 1;

    LineSelection remainder(lineWidth);
    remainder.from(nextChunkStart);
    remainder.to(to.getGlobal());
    result.push_back(remainder);
  } else if (from.getChunk().y != to.getChunk().y) {
    glm::ivec2 sameChunkEnd = to.getGlobal();
    sameChunkEnd.y = from.getGlobal().y - from.getLocal().y + (data::Chunk::SIDE_LENGTH - 1);

    LineSelection sameChunk(lineWidth);
    sameChunk.from(from.getGlobal());
    sameChunk.to(sameChunkEnd);
    result.push_back(sameChunk);

    glm::ivec2 nextChunkStart = sameChunkEnd;
    nextChunkStart.y += 1;

    LineSelection remainder(lineWidth);
    remainder.from(nextChunkStart);
    remainder.to(to.getGlobal());
    result.push_back(remainder);
  } else {
    LineSelection same(lineWidth);
    same.from(from.getGlobal());
    same.to(to.getGlobal());
    result.push_back(same);
  }

  // TODO(kantoniak): Handle case when wide line ends on the other chunk
  return result;
}

unsigned int LineSelection::getLength() const {
  const glm::ivec2 diff = toPoint - fromPoint;
  const bool goesNorth = std::abs(diff.y) > std::abs(diff.x);
  return 1 + (goesNorth ? std::abs(diff.y) : std::abs(diff.x));
}
}