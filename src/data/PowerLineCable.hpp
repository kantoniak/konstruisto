#ifndef DATA_POWERLINECABLE_HPP
#define DATA_POWERLINECABLE_HPP

#include <memory>

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "PowerLinePole.hpp"

namespace data {

class PowerLineCable {
public:
  using ptr = std::shared_ptr<PowerLineCable>;

  PowerLineCable(const PowerLinePole& pole_a, const PowerLinePole& pole_b) noexcept;
  bool operator==(const PowerLineCable& other) const noexcept;

  void update_transform() noexcept;

  [[nodiscard]] glm::mat4 get_transform() const noexcept;

private:
  const PowerLinePole& pole_a;
  const PowerLinePole& pole_b;

  glm::mat4 transform;
};
}

#endif