#include "entities/ChaserEnemy.hpp"

#include "math/VectorUtils.hpp"

namespace roboarena {

ChaserEnemy::ChaserEnemy(sf::Vector2f startPosition, const Player& target,
                         const GridMap& map)
    : Enemy(startPosition, map, kRadius, kSpeed, kHealth,
            sf::Color(235, 85, 85)),
      target_(target),
      map_(map),
      pathfinder_(map) {}

void ChaserEnemy::draw(sf::RenderWindow& window) const {
    const float markerSize = map_.getTileSize() * 0.32F;
    sf::RectangleShape marker({markerSize, markerSize});
    marker.setOrigin(markerSize * 0.5F, markerSize * 0.5F);
    marker.setFillColor(sf::Color(255, 210, 70, 95));

    for (const sf::Vector2i cell : currentPath_) {
        marker.setPosition(map_.cellToWorld(cell));
        window.draw(marker);
    }

    Enemy::draw(window);
}

sf::Vector2f ChaserEnemy::getDesiredDirection() const {
    return getNextWaypoint() - getPosition();
}

sf::Vector2f ChaserEnemy::getNextWaypoint() const {
    const sf::Vector2i enemyCell = map_.worldToCell(getPosition());
    const sf::Vector2i targetCell = map_.worldToCell(target_.getPosition());
    currentPath_ = pathfinder_.findPath(enemyCell, targetCell);

    if (currentPath_.empty()) {
        return target_.getPosition();
    }

    if (currentPath_.size() == 1U) {
        return target_.getPosition();
    }

    std::size_t waypointIndex = 1U;
    const sf::Vector2f firstWaypoint =
        map_.cellToWorld(currentPath_[waypointIndex]);

    if (math::length(firstWaypoint - getPosition()) < kWaypointSwitchDistance &&
        currentPath_.size() > 2U) {
        waypointIndex = 2U;
    }

    return map_.cellToWorld(currentPath_[waypointIndex]);
}

}  // namespace roboarena
