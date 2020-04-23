#include "Geometry.hpp"

namespace geometry {

LineSegmentOrientation get_line_segment_orientation(glm::vec2 a, glm::vec2 b, glm::vec2 c) noexcept {
  float val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
  if (val == 0) {
    return COLINEAR;
  } else if (val > 0) {
    return CLOCKWISE;
  } else {
    return COUNTER_CLOCKWISE;
  }
}

bool line_segments_intersect(glm::vec2 a1, glm::vec2 b1, glm::vec2 a4, glm::vec2 b4) noexcept {
  // Using https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/

  LineSegmentOrientation o1 = get_line_segment_orientation(a1, b1, a4);
  LineSegmentOrientation o2 = get_line_segment_orientation(a1, b1, b4);
  LineSegmentOrientation o3 = get_line_segment_orientation(a4, b4, a1);
  LineSegmentOrientation o4 = get_line_segment_orientation(a4, b4, b1);

  if (o1 != o2 && o3 != o4) {
    return true;
  }

  // If points are colinear, we say they do not intersect.
  return false;
}
}