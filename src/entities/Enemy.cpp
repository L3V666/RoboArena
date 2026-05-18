#include "entities/Enemy.hpp"

#include "math/VectorUtils.hpp"

namespace roboarena {

Enemy::Enemy(sf::Vector2f startPosition, const GridMap& map, float radius,
             float speed, sf::Color fillColor)
    : map_(map), radius_(radius), speed_(speed) {
    shape_.setRadius(radius_);
    shape_.setOrigin(radius_, radius_);
    shape_.setPosition(startPosition);
    shape_.setFillColor(fillColor);
    shape_.setOutlineColor(sf::Color(255, 220, 220));
    shape_.setOutlineThickness(2.0F);
}

void Enemy::update(float deltaTime) {
    const sf::Vector2f direction = math::normalized(getDesiredDirection());
    const sf::Vector2f offset = direction * speed_ * deltaTime;
    const sf::Vector2f nextPosition = shape_.getPosition() + offset;

    if (canMoveTo(nextPosition)) {
        shape_.move(offset);
    }
}

void Enemy::draw(sf::RenderWindow& window) const { window.draw(shape_); }

sf::FloatRect Enemy::getBounds() const { return shape_.getGlobalBounds(); }

sf::Vector2f Enemy::getPosition() const { return shape_.getPosition(); }

bool Enemy::canMoveTo(sf::Vector2f nextPosition) const {
    const sf::Vector2f left{nextPosition.x - radius_, nextPosition.y};
    const sf::Vector2f right{nextPosition.x + radius_, nextPosition.y};
    const sf::Vector2f top{nextPosition.x, nextPosition.y - radius_};
    const sf::Vector2f bottom{nextPosition.x, nextPosition.y + radius_};

    return map_.isWalkablePixel(left) && map_.isWalkablePixel(right) &&
           map_.isWalkablePixel(top) && map_.isWalkablePixel(bottom);
}

}  // namespace roboarena
