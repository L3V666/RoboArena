#ifndef ROBOARENA_ENTITIES_ENEMY_HPP
#define ROBOARENA_ENTITIES_ENEMY_HPP

#include <SFML/Graphics.hpp>

#include "entities/Entity.hpp"
#include "map/GridMap.hpp"

namespace roboarena {

class Enemy : public Entity {
   public:
    Enemy(sf::Vector2f startPosition, const GridMap& map, float radius,
          float speed, sf::Color fillColor);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) const override;
    sf::FloatRect getBounds() const override;

    [[nodiscard]] sf::Vector2f getPosition() const;

   protected:
    [[nodiscard]] virtual sf::Vector2f getDesiredDirection() const = 0;

   private:
    [[nodiscard]] bool canMoveTo(sf::Vector2f nextPosition) const;

    sf::CircleShape shape_;
    const GridMap& map_;
    float radius_;
    float speed_;
};

}  // namespace roboarena

#endif  // ROBOARENA_ENTITIES_ENEMY_HPP
