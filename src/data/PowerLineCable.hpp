#ifndef DATA_POWERLINECABLE_HPP
#define DATA_POWERLINECABLE_HPP

#include <memory>
#include <vector>

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "../geometry/utils.hpp"
#include "PowerLinePole.hpp"

namespace data {

class PowerLineCable {
public:
  using ptr = std::shared_ptr<PowerLineCable>;

  PowerLineCable(const PowerLinePole& pole_a, const PowerLinePole& pole_b) noexcept;
  bool operator==(const PowerLineCable& other) const noexcept;

  void update_transform() noexcept;

  [[nodiscard]] const std::vector<glm::mat4>& get_transforms() const noexcept;

private:
  const PowerLinePole& pole_a;
  const PowerLinePole& pole_b;

  const size_t cable_count;
  std::vector<glm::mat4> transforms;
};
}

#endif