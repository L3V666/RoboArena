#ifndef ROBOARENA_ENTITIES_CHASER_ENEMY_HPP
#define ROBOARENA_ENTITIES_CHASER_ENEMY_HPP

#include <SFML/Graphics.hpp>

#include "entities/Enemy.hpp"
#include "entities/Player.hpp"
#include "map/GridMap.hpp"

namespace roboarena {

class ChaserEnemy final : public Enemy {
   public:
    ChaserEnemy(sf::Vector2f startPosition, const Player& target,
                const GridMap& map);

   protected:
    [[nodiscard]] sf::Vector2f getDesiredDirection() const override;

   private:
    static constexpr float kRadius = 15.0F;
    static constexpr float kSpeed = 125.0F;

    const Player& target_;
};

}  // namespace roboarena

#endif  // ROBOARENA_ENTITIES_CHASER_ENEMY_HPP
