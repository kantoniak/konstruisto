#ifndef INPUT_SELECTION_HPP
#define INPUT_SELECTION_HPP

#include <glm/glm.hpp>

namespace input {

class Selection {

public:
  Selection();

  void start(glm::ivec2 point);
  void from(glm::ivec2 point);
  void to(glm::ivec2 point);
  void stop();

  bool isSelecting();
  void reset();

  glm::ivec2 getFrom();
  glm::ivec2 getTo();

private:
  bool selecting;
  glm::ivec2 fromPoint;
  glm::ivec2 toPoint;

  void updateFromAndTo();
};
}

#endif
