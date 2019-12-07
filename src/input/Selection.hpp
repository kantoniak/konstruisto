#ifndef INPUT_SELECTION_HPP
#define INPUT_SELECTION_HPP

#include <glm/glm.hpp>
#include <vector>

#include "../data/Position.hpp"

namespace input {

class Selection {

public:
  Selection();
  Selection& operator=(const Selection& other);
  virtual ~Selection() = default;

  void setColors(glm::vec4 startColor, glm::vec4 selectionColor, glm::vec4 invalidColor);
  [[nodiscard]] glm::vec4 getColor() const;

  void start(glm::ivec2 point);
  void from(glm::ivec2 point);
  void to(glm::ivec2 point);
  void stop();

  void markInvalid();
  void markValid();
  [[nodiscard]] bool isValid() const;

  [[nodiscard]] bool isSelecting() const;
  void reset();

  [[nodiscard]] virtual glm::ivec2 getFrom() const;
  [[nodiscard]] virtual glm::ivec2 getTo() const;
  [[nodiscard]] virtual const std::vector<data::Position<int32_t>> getSelected() const;

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
