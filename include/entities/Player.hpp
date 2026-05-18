#ifndef ROBOARENA_ENTITIES_PLAYER_HPP
#define ROBOARENA_ENTITIES_PLAYER_HPP

#include <SFML/Graphics.hpp>

#include "entities/Entity.hpp"
#include "map/GridMap.hpp"

namespace roboarena {

class Player final : public Entity {
   public:
    explicit Player(sf::Vector2f startPosition);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) const override;
    [[nodiscard]] sf::FloatRect getBounds() const override;

    void setMap(const GridMap* map);
    void takeDamage(int damage);

    [[nodiscard]] sf::Vector2f getPosition() const;
    [[nodiscard]] int getHealth() const;
    [[nodiscard]] int getMaxHealth() const;
    [[nodiscard]] bool isDefeated() const;

   private:
    static constexpr float kRadius = 18.0F;
    static constexpr float kSpeed = 260.0F;
    static constexpr int kMaxHealth = 100;

    void handleMovement(float deltaTime);
    [[nodiscard]] bool canMoveTo(sf::Vector2f nextPosition) const;

    sf::CircleShape shape_;
    const GridMap* map_ = nullptr;
    int health_ = kMaxHealth;
};

}  // namespace roboarena

#endif  // ROBOARENA_ENTITIES_PLAYER_HPP
