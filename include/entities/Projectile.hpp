#ifndef ROBOARENA_ENTITIES_PROJECTILE_HPP
#define ROBOARENA_ENTITIES_PROJECTILE_HPP

#include <SFML/Graphics.hpp>

#include "entities/Entity.hpp"
#include "map/GridMap.hpp"

namespace roboarena {

class Projectile final : public Entity {
   public:
    Projectile(sf::Vector2f startPosition, sf::Vector2f direction,
               const GridMap& map);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) const override;
    [[nodiscard]] sf::FloatRect getBounds() const override;
    [[nodiscard]] bool shouldBeRemoved() const override;

    void destroy();

    [[nodiscard]] int getDamage() const;

   private:
    static constexpr float kRadius = 5.0F;
    static constexpr float kSpeed = 520.0F;
    static constexpr float kMaxLifetime = 1.4F;
    static constexpr int kDamage = 30;

    sf::CircleShape shape_;
    sf::Vector2f velocity_;
    const GridMap& map_;
    float lifetime_ = 0.0F;
    bool destroyed_ = false;
};

}  // namespace roboarena

#endif  // ROBOARENA_ENTITIES_PROJECTILE_HPP
