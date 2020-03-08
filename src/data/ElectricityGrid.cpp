#include "ElectricityGrid.hpp"

#include <memory>

namespace data {
ElectricityGrid::ElectricityGrid() noexcept = default;

void ElectricityGrid::add_pole(PowerLinePole::ptr pole) noexcept {
  poles.push_back(pole);
}

bool ElectricityGrid::remove_pole(const PowerLinePole& pole) noexcept {
  const auto& to_remove =
      std::find_if(poles.begin(), poles.end(), [&](const PowerLinePole::ptr& ptr) { return *ptr == pole; });

  if (to_remove == poles.end()) {
    return false;
  }

  poles.erase(to_remove);
  return true;
}

size_t ElectricityGrid::get_pole_count() const noexcept {
  return poles.size();
}

const std::list<PowerLinePole::ptr>& ElectricityGrid::get_poles() const noexcept {
  return poles;
}

void ElectricityGrid::add_cable(PowerLineCable::ptr cable) noexcept {
  cables.push_back(cable);
}

bool ElectricityGrid::remove_cable(const PowerLineCable& cable) noexcept {
  const auto& to_remove =
      std::find_if(cables.begin(), cables.end(), [&](const PowerLineCable::ptr& ptr) { return *ptr == cable; });

  if (to_remove == cables.end()) {
    return false;
  }

  cables.erase(to_remove);
  return true;
}

size_t ElectricityGrid::get_cable_count() const noexcept {
  return cables.size();
}

const std::list<PowerLineCable::ptr>& ElectricityGrid::get_cables() const noexcept {
  return cables;
}
}