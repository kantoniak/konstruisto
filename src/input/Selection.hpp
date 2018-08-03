#ifndef INPUT_SELECTION_HPP
#define INPUT_SELECTION_HPP

#include <glm/glm.hpp>

namespace input {

class Selection {

public:
  Selection();
  Selection& operator=(const Selection& other);
  virtual ~Selection() = default;

  void setColors(glm::vec4 startColor, glm::vec4 selectionColor, glm::vec4 invalidColor);
  glm::vec4 getColor() const;

  void start(glm::ivec2 point);
  void from(glm::ivec2 point);
  void to(glm::ivec2 point);
  void stop();

  void markInvalid();
  void markValid();
  bool isValid() const;

  bool isSelecting() const;
  void reset();

  virtual glm::ivec2 getFrom() const;
  virtual glm::ivec2 getTo() const;

protected:
  bool selecting;
  bool valid;
  glm::ivec2 fromPoint;
  glm::ivec2 toPoint;

  glm::vec4 startColor;
  glm::vec4 selectionColor;
  glm::vec4 invalidColor;
};
}

#endif
