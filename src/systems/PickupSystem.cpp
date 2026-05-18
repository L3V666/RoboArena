#include "systems/PickupSystem.hpp"

#include <algorithm>
#include <array>

namespace roboarena {

PickupSystem::PickupSystem()
    : spawnPoints_{sf::Vector2f{176.0F, 112.0F}, sf::Vector2f{816.0F, 112.0F},
                   sf::Vector2f{144.0F, 496.0F}, sf::Vector2f{496.0F, 528.0F},
                   sf::Vector2f{592.0F, 304.0F}, sf::Vector2f{272.0F, 304.0F}} {
}

void PickupSystem::reset() {
    nextSpawnPointIndex_ = 0;
    nextPowerUpTypeIndex_ = 0;
    spawnTimer_ = 2.0F;
}

void PickupSystem::update(EntityManager& entityManager, Player& player,
                          float deltaTime) {
    collectTouchedPowerUps(entityManager, player);

    spawnTimer_ = std::max(0.0F, spawnTimer_ - deltaTime);

    if (spawnTimer_ > 0.0F) {
        return;
    }

    if (getActivePowerUpCount(entityManager) >= kMaxActivePowerUps) {
        spawnTimer_ = kSpawnInterval * 0.5F;
        return;
    }

    spawnPowerUp(entityManager);
    spawnTimer_ = kSpawnInterval;
}

std::size_t PickupSystem::getActivePowerUpCount(
    EntityManager& entityManager) const {
    const std::vector<PowerUp*> powerUps = entityManager.getAll<PowerUp>();
    std::size_t count = 0;

    for (const PowerUp* powerUp : powerUps) {
        if (!powerUp->shouldBeRemoved()) {
            ++count;
        }
    }

    return count;
}

float PickupSystem::getNextSpawnRatio() const {
    return 1.0F - std::clamp(spawnTimer_ / kSpawnInterval, 0.0F, 1.0F);
}

void PickupSystem::spawnPowerUp(EntityManager& entityManager) {
    entityManager.create<PowerUp>(getNextSpawnPoint(), getNextPowerUpType());
}

void PickupSystem::collectTouchedPowerUps(EntityManager& entityManager,
                                          Player& player) const {
    const std::vector<PowerUp*> powerUps = entityManager.getAll<PowerUp>();

    for (PowerUp* powerUp : powerUps) {
        if (powerUp->shouldBeRemoved()) {
            continue;
        }

        if (!powerUp->getBounds().intersects(player.getBounds())) {
            continue;
        }

        applyPowerUp(player, powerUp->getType());
        powerUp->destroy();
    }
}

void PickupSystem::applyPowerUp(Player& player, PowerUpType type) const {
    switch (type) {
        case PowerUpType::Heal:
            player.heal(kHealAmount);
            break;
        case PowerUpType::Speed:
            player.activateSpeedBoost(kSpeedBoostDuration,
                                      kSpeedBoostMultiplier);
            break;
        case PowerUpType::RapidFire:
            player.activateRapidFire(kRapidFireDuration);
            break;
    }
}

sf::Vector2f PickupSystem::getNextSpawnPoint() {
    const sf::Vector2f spawnPoint = spawnPoints_[nextSpawnPointIndex_];
    nextSpawnPointIndex_ = (nextSpawnPointIndex_ + 1U) % spawnPoints_.size();
    return spawnPoint;
}

PowerUpType PickupSystem::getNextPowerUpType() {
    static const std::array<PowerUpType, 3U> kTypes{
        PowerUpType::Heal, PowerUpType::Speed, PowerUpType::RapidFire};

    const PowerUpType type = kTypes[nextPowerUpTypeIndex_];
    nextPowerUpTypeIndex_ = (nextPowerUpTypeIndex_ + 1U) % kTypes.size();
    return type;
}

}  // namespace roboarena
