#ifndef ROBOARENA_ENTITIES_ENEMY_HPP
#define ROBOARENA_ENTITIES_ENEMY_HPP

#include <SFML/Graphics.hpp>

#include "entities/Entity.hpp"
#include "map/GridMap.hpp"

namespace roboarena {

class Enemy : public Entity {
   public:
    Enemy(sf::Vector2f startPosition, const GridMap& map, float radius,
          float speed, int health, sf::Color fillColor);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) const override;
    [[nodiscard]] sf::FloatRect getBounds() const override;
    [[nodiscard]] bool shouldBeRemoved() const override;

    void takeDamage(int damage);

    [[nodiscard]] sf::Vector2f getPosition() const;
    [[nodiscard]] int getHealth() const;
    [[nodiscard]] int getMaxHealth() const;
    [[nodiscard]] int getContactDamage() const;

   protected:
    [[nodiscard]] virtual sf::Vector2f getDesiredDirection() const = 0;

   private:
    static constexpr int kContactDamage = 12;
    static constexpr float kMinMovementLength = 0.0001F;

    [[nodiscard]] bool tryMove(sf::Vector2f offset);
    [[nodiscard]] bool canMoveTo(sf::Vector2f nextPosition) const;
    void drawHealthBar(sf::RenderWindow& window) const;

    sf::CircleShape shape_;
    const GridMap& map_;
    float radius_;
    float speed_;
    int health_;
    int maxHealth_;
};

}  // namespace roboarena

#endif  // ROBOARENA_ENTITIES_ENEMY_HPP
