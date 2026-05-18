#include "entities/Enemy.hpp"

#include <algorithm>
#include <array>
#include <cmath>

#include "math/VectorUtils.hpp"

namespace roboarena {

Enemy::Enemy(sf::Vector2f startPosition, const GridMap& map, float radius,
             float speed, int health, sf::Color fillColor)
    : map_(map),
      radius_(radius),
      speed_(speed),
      health_(health),
      maxHealth_(health) {
    shape_.setRadius(radius_);
    shape_.setOrigin(radius_, radius_);
    shape_.setPosition(startPosition);
    shape_.setFillColor(fillColor);
    shape_.setOutlineColor(sf::Color(255, 220, 220));
    shape_.setOutlineThickness(2.0F);
}

void Enemy::update(float deltaTime) {
    if (shouldBeRemoved()) {
        return;
    }

    const sf::Vector2f direction = math::normalized(getDesiredDirection());
    const sf::Vector2f offset = direction * speed_ * deltaTime;

    if (math::length(offset) <= kMinMovementLength) {
        return;
    }

    if (tryMove(offset)) {
        return;
    }

    const sf::Vector2f horizontalOffset{offset.x, 0.0F};
    const sf::Vector2f verticalOffset{0.0F, offset.y};

    if (std::abs(offset.x) > std::abs(offset.y)) {
        if (!tryMove(horizontalOffset)) {
            const bool movedVertically = tryMove(verticalOffset);
            (void)movedVertically;
        }
    } else {
        if (!tryMove(verticalOffset)) {
            const bool movedHorizontally = tryMove(horizontalOffset);
            (void)movedHorizontally;
        }
    }
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(shape_);
    drawHealthBar(window);
}

sf::FloatRect Enemy::getBounds() const { return shape_.getGlobalBounds(); }

bool Enemy::shouldBeRemoved() const { return health_ <= 0; }

void Enemy::takeDamage(int damage) {
    health_ = std::max(0, health_ - std::max(0, damage));
}

sf::Vector2f Enemy::getPosition() const { return shape_.getPosition(); }

int Enemy::getHealth() const { return health_; }

int Enemy::getMaxHealth() const { return maxHealth_; }

int Enemy::getContactDamage() const { return kContactDamage; }

bool Enemy::tryMove(sf::Vector2f offset) {
    if (math::length(offset) <= kMinMovementLength) {
        return false;
    }

    const sf::Vector2f nextPosition = shape_.getPosition() + offset;

    if (!canMoveTo(nextPosition)) {
        return false;
    }

    shape_.move(offset);
    return true;
}

bool Enemy::canMoveTo(sf::Vector2f nextPosition) const {
    const float diagonalOffset = radius_ * 0.70710678F;
    const std::array<sf::Vector2f, 8> checkPoints{
        sf::Vector2f{nextPosition.x - radius_, nextPosition.y},
        sf::Vector2f{nextPosition.x + radius_, nextPosition.y},
        sf::Vector2f{nextPosition.x, nextPosition.y - radius_},
        sf::Vector2f{nextPosition.x, nextPosition.y + radius_},
        sf::Vector2f{nextPosition.x - diagonalOffset,
                     nextPosition.y - diagonalOffset},
        sf::Vector2f{nextPosition.x + diagonalOffset,
                     nextPosition.y - diagonalOffset},
        sf::Vector2f{nextPosition.x - diagonalOffset,
                     nextPosition.y + diagonalOffset},
        sf::Vector2f{nextPosition.x + diagonalOffset,
                     nextPosition.y + diagonalOffset}};

    for (const sf::Vector2f point : checkPoints) {
        if (!map_.isWalkablePixel(point)) {
            return false;
        }
    }

    return true;
}

void Enemy::drawHealthBar(sf::RenderWindow& window) const {
    if (maxHealth_ <= 0) {
        return;
    }

    constexpr float kBarWidth = 34.0F;
    constexpr float kBarHeight = 4.0F;
    constexpr float kBarOffsetY = 24.0F;

    const float healthRatio =
        static_cast<float>(health_) / static_cast<float>(maxHealth_);
    const sf::Vector2f position = shape_.getPosition();

    sf::RectangleShape background({kBarWidth, kBarHeight});
    background.setOrigin(kBarWidth * 0.5F, kBarHeight * 0.5F);
    background.setPosition(position.x, position.y - kBarOffsetY);
    background.setFillColor(sf::Color(70, 20, 20));

    sf::RectangleShape foreground({kBarWidth * healthRatio, kBarHeight});
    foreground.setOrigin(kBarWidth * 0.5F, kBarHeight * 0.5F);
    foreground.setPosition(
        position.x - (kBarWidth - kBarWidth * healthRatio) * 0.5F,
        position.y - kBarOffsetY);
    foreground.setFillColor(sf::Color(70, 220, 90));

    window.draw(background);
    window.draw(foreground);
}

}  // namespace roboarena
