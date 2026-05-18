#include "entities/Projectile.hpp"

#include "math/VectorUtils.hpp"

namespace roboarena {

Projectile::Projectile(sf::Vector2f startPosition, sf::Vector2f direction,
                       const GridMap& map)
    : velocity_(math::normalized(direction) * kSpeed), map_(map) {
    shape_.setRadius(kRadius);
    shape_.setOrigin(kRadius, kRadius);
    shape_.setPosition(startPosition);
    shape_.setFillColor(sf::Color(255, 240, 120));
    shape_.setOutlineColor(sf::Color(255, 255, 220));
    shape_.setOutlineThickness(1.0F);

    if (math::length(direction) <= 0.0001F) {
        destroyed_ = true;
    }
}

void Projectile::update(float deltaTime) {
    if (destroyed_) {
        return;
    }

    lifetime_ += deltaTime;
    shape_.move(velocity_ * deltaTime);

    if (lifetime_ >= kMaxLifetime ||
        !map_.isWalkablePixel(shape_.getPosition())) {
        destroy();
    }
}

void Projectile::draw(sf::RenderWindow& window) const { window.draw(shape_); }

sf::FloatRect Projectile::getBounds() const { return shape_.getGlobalBounds(); }

bool Projectile::shouldBeRemoved() const { return destroyed_; }

void Projectile::destroy() { destroyed_ = true; }

int Projectile::getDamage() const { return kDamage; }

}  // namespace roboarena
