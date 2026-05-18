#include "math/VectorUtils.hpp"

#include <cmath>

namespace roboarena::math {

float length(sf::Vector2f vector) {
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

sf::Vector2f normalized(sf::Vector2f vector) {
    const float vectorLength = length(vector);

    if (vectorLength <= 0.0001F) {
        return {0.0F, 0.0F};
    }

    return {vector.x / vectorLength, vector.y / vectorLength};
}

} // namespace roboarena::math
