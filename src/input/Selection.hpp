#ifndef INPUT_SELECTION_HPP
#define INPUT_SELECTION_HPP

#include <glm/glm.hpp>

namespace input {

class Selection {

public:
  Selection();

  void setColors(glm::vec4 startColor, glm::vec4 selectionColor, glm::vec4 invalidColor);
  glm::vec4 getColor();

  void start(glm::ivec2 point);
  void from(glm::ivec2 point);
  void to(glm::ivec2 point);
  void stop();

  void markInvalid();
  void markValid();
  bool isValid();

  bool isSelecting();
  void reset();

  glm::ivec2 getFrom();
  glm::ivec2 getTo();

private:
  bool selecting;
  bool valid;
  glm::ivec2 fromPoint;
  glm::ivec2 toPoint;

  glm::vec4 startColor;
  glm::vec4 selectionColor;
  glm::vec4 invalidColor;

  void updateFromAndTo();
};
}

#endif
