#ifndef ROBOARENA_ENTITIES_ENTITY_HPP
#define ROBOARENA_ENTITIES_ENTITY_HPP

#include <SFML/Graphics.hpp>

namespace roboarena {

class Entity {
public:
    virtual ~Entity();

    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;

    [[nodiscard]] virtual sf::FloatRect getBounds() const = 0;
    [[nodiscard]] virtual bool shouldBeRemoved() const;
};

} // namespace roboarena

#endif // ROBOARENA_ENTITIES_ENTITY_HPP
