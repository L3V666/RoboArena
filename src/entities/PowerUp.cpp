#include "entities/PowerUp.hpp"

#include <algorithm>
#include <cmath>

namespace roboarena {

PowerUp::PowerUp(sf::Vector2f position, PowerUpType type) : type_(type) {
    shape_.setRadius(kRadius);
    shape_.setPointCount(6U);
    shape_.setOrigin(kRadius, kRadius);
    shape_.setPosition(position);
    shape_.setFillColor(getFillColor(type_));
    shape_.setOutlineColor(getOutlineColor(type_));
    shape_.setOutlineThickness(2.0F);
}

void PowerUp::update(float deltaTime) {
    if (collected_) {
        return;
    }

    lifetime_ += deltaTime;
    pulse_ += deltaTime * kPulseSpeed;

    const float scale = 1.0F + std::sin(pulse_) * 0.10F;
    shape_.setScale(scale, scale);

    if (lifetime_ >= kLifetime) {
        destroy();
    }
}

void PowerUp::draw(sf::RenderWindow& window) const { window.draw(shape_); }

sf::FloatRect PowerUp::getBounds() const { return shape_.getGlobalBounds(); }

bool PowerUp::shouldBeRemoved() const {
    return collected_ || lifetime_ >= kLifetime;
}

void PowerUp::destroy() { collected_ = true; }

PowerUpType PowerUp::getType() const { return type_; }

float PowerUp::getRemainingLifetimeRatio() const {
    return std::clamp(1.0F - lifetime_ / kLifetime, 0.0F, 1.0F);
}

sf::Color PowerUp::getFillColor(PowerUpType type) {
    switch (type) {
        case PowerUpType::Heal:
            return sf::Color(80, 230, 110);
        case PowerUpType::Speed:
            return sf::Color(95, 170, 255);
        case PowerUpType::RapidFire:
            return sf::Color(255, 210, 85);
    }

    return sf::Color::White;
}

sf::Color PowerUp::getOutlineColor(PowerUpType type) {
    switch (type) {
        case PowerUpType::Heal:
            return sf::Color(210, 255, 220);
        case PowerUpType::Speed:
            return sf::Color(220, 238, 255);
        case PowerUpType::RapidFire:
            return sf::Color(255, 245, 190);
    }

    return sf::Color::White;
}

}  // namespace roboarena
