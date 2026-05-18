#include "entities/Player.hpp"

#include "math/VectorUtils.hpp"

namespace roboarena {

Player::Player(sf::Vector2f startPosition) {
    shape_.setRadius(kRadius);
    shape_.setOrigin(kRadius, kRadius);
    shape_.setPosition(startPosition);
    shape_.setFillColor(sf::Color(80, 190, 255));
    shape_.setOutlineColor(sf::Color(210, 245, 255));
    shape_.setOutlineThickness(2.0F);
}

void Player::update(float deltaTime) { handleMovement(deltaTime); }

void Player::draw(sf::RenderWindow& window) const { window.draw(shape_); }

sf::FloatRect Player::getBounds() const { return shape_.getGlobalBounds(); }

void Player::setMap(const GridMap* map) { map_ = map; }

sf::Vector2f Player::getPosition() const { return shape_.getPosition(); }

void Player::handleMovement(float deltaTime) {
    sf::Vector2f direction{0.0F, 0.0F};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        direction.y -= 1.0F;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        direction.y += 1.0F;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        direction.x -= 1.0F;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        direction.x += 1.0F;
    }

    direction = math::normalized(direction);
    const sf::Vector2f offset = direction * kSpeed * deltaTime;
    const sf::Vector2f nextPosition = shape_.getPosition() + offset;

    if (canMoveTo(nextPosition)) {
        shape_.move(offset);
    }
}

bool Player::canMoveTo(sf::Vector2f nextPosition) const {
    if (map_ == nullptr) {
        return true;
    }

    const sf::Vector2f left{nextPosition.x - kRadius, nextPosition.y};
    const sf::Vector2f right{nextPosition.x + kRadius, nextPosition.y};
    const sf::Vector2f top{nextPosition.x, nextPosition.y - kRadius};
    const sf::Vector2f bottom{nextPosition.x, nextPosition.y + kRadius};

    return map_->isWalkablePixel(left) && map_->isWalkablePixel(right) &&
           map_->isWalkablePixel(top) && map_->isWalkablePixel(bottom);
}

}  // namespace roboarena
