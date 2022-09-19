#include "./number.hpp"
#include <climits>
#include <cmath>
#include <limits>

namespace utils::number {

bool compare_float(float f1, float f2) noexcept {
  return std::fabs(f1 - f2) < std::numeric_limits<float>::epsilon();
}

bool compare_double(double d1, double d2) noexcept {
  return std::fabs(d1 - d2) < std::numeric_limits<double>::epsilon();
}

} // namespace utils::number
