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
    sf::FloatRect getBounds() const override;

    void setMap(const GridMap* map);
    sf::Vector2f getPosition() const;

   private:
    static constexpr float kRadius = 18.0F;
    static constexpr float kSpeed = 260.0F;

    void handleMovement(float deltaTime);
    bool canMoveTo(sf::Vector2f nextPosition) const;

    sf::CircleShape shape_;
    const GridMap* map_ = nullptr;
};

}  // namespace roboarena

#endif  // ROBOARENA_ENTITIES_PLAYER_HPP
