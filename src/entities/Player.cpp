#include "entities/Player.hpp"

#include <algorithm>
#include <array>

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

void Player::update(float deltaTime) {
    updatePowerUps(deltaTime);

    if (isDefeated()) {
        return;
    }

    handleMovement(deltaTime);
}

void Player::draw(sf::RenderWindow& window) const { window.draw(shape_); }

sf::FloatRect Player::getBounds() const { return shape_.getGlobalBounds(); }

void Player::setMap(const GridMap* map) { map_ = map; }

void Player::takeDamage(int damage) {
    health_ = std::max(0, health_ - std::max(0, damage));
}

void Player::heal(int amount) {
    health_ = std::min(kMaxHealth, health_ + std::max(0, amount));
}

void Player::activateSpeedBoost(float duration, float multiplier) {
    speedBoostDuration_ = std::max(0.1F, duration);
    speedBoostTimer_ = speedBoostDuration_;
    speedBoostMultiplier_ = std::max(1.0F, multiplier);
}

void Player::activateRapidFire(float duration) {
    rapidFireDuration_ = std::max(0.1F, duration);
    rapidFireTimer_ = rapidFireDuration_;
}

sf::Vector2f Player::getPosition() const { return shape_.getPosition(); }

int Player::getHealth() const { return health_; }

int Player::getMaxHealth() const { return kMaxHealth; }

bool Player::isDefeated() const { return health_ <= 0; }

float Player::getShootCooldownMultiplier() const {
    return rapidFireTimer_ > 0.0F ? kRapidFireCooldownMultiplier : 1.0F;
}

float Player::getSpeedBoostRatio() const {
    return std::clamp(speedBoostTimer_ / speedBoostDuration_, 0.0F, 1.0F);
}

float Player::getRapidFireRatio() const {
    return std::clamp(rapidFireTimer_ / rapidFireDuration_, 0.0F, 1.0F);
}

void Player::updatePowerUps(float deltaTime) {
    speedBoostTimer_ = std::max(0.0F, speedBoostTimer_ - deltaTime);
    rapidFireTimer_ = std::max(0.0F, rapidFireTimer_ - deltaTime);

    if (speedBoostTimer_ <= 0.0F) {
        speedBoostMultiplier_ = 1.0F;
    }
}

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
    const sf::Vector2f offset = direction * getCurrentSpeed() * deltaTime;
    const sf::Vector2f nextPosition = shape_.getPosition() + offset;

    if (canMoveTo(nextPosition)) {
        shape_.move(offset);
    }
}

float Player::getCurrentSpeed() const { return kSpeed * speedBoostMultiplier_; }

bool Player::canMoveTo(sf::Vector2f nextPosition) const {
    if (map_ == nullptr) {
        return true;
    }

    const float diagonalOffset = kRadius * 0.70710678F;
    const std::array<sf::Vector2f, 8> checkPoints{
        sf::Vector2f{nextPosition.x - kRadius, nextPosition.y},
        sf::Vector2f{nextPosition.x + kRadius, nextPosition.y},
        sf::Vector2f{nextPosition.x, nextPosition.y - kRadius},
        sf::Vector2f{nextPosition.x, nextPosition.y + kRadius},
        sf::Vector2f{nextPosition.x - diagonalOffset,
                     nextPosition.y - diagonalOffset},
        sf::Vector2f{nextPosition.x + diagonalOffset,
                     nextPosition.y - diagonalOffset},
        sf::Vector2f{nextPosition.x - diagonalOffset,
                     nextPosition.y + diagonalOffset},
        sf::Vector2f{nextPosition.x + diagonalOffset,
                     nextPosition.y + diagonalOffset}};

    for (const sf::Vector2f point : checkPoints) {
        if (!map_->isWalkablePixel(point)) {
            return false;
        }
    }

    return true;
}

}  // namespace roboarena
