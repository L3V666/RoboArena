#ifndef ROBOARENA_MATH_VECTOR_UTILS_HPP
#define ROBOARENA_MATH_VECTOR_UTILS_HPP

#include <SFML/System/Vector2.hpp>

namespace roboarena::math {

[[nodiscard]] float length(sf::Vector2f vector);
[[nodiscard]] sf::Vector2f normalized(sf::Vector2f vector);

}  // namespace roboarena::math

#endif  // ROBOARENA_MATH_VECTOR_UTILS_HPP
