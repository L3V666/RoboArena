#include "entities/ChaserEnemy.hpp"

namespace roboarena {

ChaserEnemy::ChaserEnemy(sf::Vector2f startPosition, const Player& target,
                         const GridMap& map)
    : Enemy(startPosition, map, kRadius, kSpeed, sf::Color(235, 85, 85)),
      target_(target) {}

sf::Vector2f ChaserEnemy::getDesiredDirection() const {
    return target_.getPosition() - getPosition();
}

}  // namespace roboarena
