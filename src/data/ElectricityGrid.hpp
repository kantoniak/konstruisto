#ifndef DATA_ELECTRICITYGRID_HPP
#define DATA_ELECTRICITYGRID_HPP

#include <algorithm>
#include <array>
#include <list>
#include <memory>

#include "PowerLineCable.hpp"
#include "PowerLinePole.hpp"

namespace data {

class ElectricityGrid {
public:
  ElectricityGrid() noexcept;

  // Poles
  void add_pole(PowerLinePole::ptr pole) noexcept;
  bool remove_pole(const PowerLinePole& pole) noexcept;

  [[nodiscard]] size_t get_pole_count() const noexcept;
  [[nodiscard]] const std::list<PowerLinePole::ptr>& get_poles() const noexcept;

  // Cables
  void add_cable(PowerLineCable::ptr cable) noexcept;
  bool remove_cable(const PowerLineCable& cable) noexcept;

  [[nodiscard]] size_t get_cable_count() const noexcept;
  [[nodiscard]] const std::list<PowerLineCable::ptr>& get_cables() const noexcept;

private:
  std::list<PowerLinePole::ptr> poles;
  std::list<PowerLineCable::ptr> cables;
};
}

#endif