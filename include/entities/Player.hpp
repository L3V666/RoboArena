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
    void heal(int amount);
    void activateSpeedBoost(float duration, float multiplier);
    void activateRapidFire(float duration);

    [[nodiscard]] sf::Vector2f getPosition() const;
    [[nodiscard]] int getHealth() const;
    [[nodiscard]] int getMaxHealth() const;
    [[nodiscard]] bool isDefeated() const;
    [[nodiscard]] float getShootCooldownMultiplier() const;
    [[nodiscard]] float getSpeedBoostRatio() const;
    [[nodiscard]] float getRapidFireRatio() const;

   private:
    static constexpr float kRadius = 18.0F;
    static constexpr float kSpeed = 260.0F;
    static constexpr int kMaxHealth = 100;
    static constexpr float kRapidFireCooldownMultiplier = 0.45F;

    void updatePowerUps(float deltaTime);
    void handleMovement(float deltaTime);
    [[nodiscard]] float getCurrentSpeed() const;
    [[nodiscard]] bool canMoveTo(sf::Vector2f nextPosition) const;

    sf::CircleShape shape_;
    const GridMap* map_ = nullptr;
    int health_ = kMaxHealth;
    float speedBoostTimer_ = 0.0F;
    float speedBoostDuration_ = 1.0F;
    float speedBoostMultiplier_ = 1.0F;
    float rapidFireTimer_ = 0.0F;
    float rapidFireDuration_ = 1.0F;
};

}  // namespace roboarena

#endif  // ROBOARENA_ENTITIES_PLAYER_HPP
