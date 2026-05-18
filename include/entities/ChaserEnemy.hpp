#ifndef ROBOARENA_ENTITIES_CHASER_ENEMY_HPP
#define ROBOARENA_ENTITIES_CHASER_ENEMY_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "entities/Enemy.hpp"
#include "entities/Player.hpp"
#include "map/GridMap.hpp"
#include "systems/Pathfinder.hpp"

namespace roboarena {

class ChaserEnemy final : public Enemy {
   public:
    ChaserEnemy(sf::Vector2f startPosition, const Player& target,
                const GridMap& map);

    void draw(sf::RenderWindow& window) const override;

   protected:
    [[nodiscard]] sf::Vector2f getDesiredDirection() const override;

   private:
    static constexpr float kRadius = 12.0F;
    static constexpr float kSpeed = 125.0F;
    static constexpr int kHealth = 90;
    static constexpr float kWaypointSwitchDistance = 10.0F;

    [[nodiscard]] sf::Vector2f getNextWaypoint() const;

    const Player& target_;
    const GridMap& map_;
    Pathfinder pathfinder_;
    mutable std::vector<sf::Vector2i> currentPath_;
};

}  // namespace roboarena

#endif  // ROBOARENA_ENTITIES_CHASER_ENEMY_HPP
