#ifndef GEOMETRY_UTILS_HPP
#define GEOMETRY_UTILS_HPP

#include <glm/glm.hpp>

namespace geometry {

enum LineSegmentOrientation { COLINEAR, CLOCKWISE, COUNTER_CLOCKWISE };

LineSegmentOrientation get_line_segment_orientation(glm::vec2 a, glm::vec2 b, glm::vec2 c) noexcept;
bool line_segments_intersect(glm::vec2 a1, glm::vec2 b1, glm::vec2 a4, glm::vec2 b4) noexcept;

}

#endif