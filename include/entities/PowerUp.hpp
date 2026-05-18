#ifndef ROBOARENA_ENTITIES_POWER_UP_HPP
#define ROBOARENA_ENTITIES_POWER_UP_HPP

#include <SFML/Graphics.hpp>

#include "entities/Entity.hpp"

namespace roboarena {

enum class PowerUpType { Heal, Speed, RapidFire };

class PowerUp final : public Entity {
   public:
    PowerUp(sf::Vector2f position, PowerUpType type);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) const override;
    [[nodiscard]] sf::FloatRect getBounds() const override;
    [[nodiscard]] bool shouldBeRemoved() const override;

    void destroy();

    [[nodiscard]] PowerUpType getType() const;
    [[nodiscard]] float getRemainingLifetimeRatio() const;

   private:
    static constexpr float kRadius = 12.0F;
    static constexpr float kLifetime = 12.0F;
    static constexpr float kPulseSpeed = 5.5F;

    [[nodiscard]] static sf::Color getFillColor(PowerUpType type);
    [[nodiscard]] static sf::Color getOutlineColor(PowerUpType type);

    sf::CircleShape shape_;
    PowerUpType type_;
    float lifetime_ = 0.0F;
    float pulse_ = 0.0F;
    bool collected_ = false;
};

}  // namespace roboarena

#endif  // ROBOARENA_ENTITIES_POWER_UP_HPP
