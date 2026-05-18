#ifndef ROBOARENA_SYSTEMS_PICKUP_SYSTEM_HPP
#define ROBOARENA_SYSTEMS_PICKUP_SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <vector>

#include "entities/Player.hpp"
#include "entities/PowerUp.hpp"
#include "systems/EntityManager.hpp"

namespace roboarena {

class PickupSystem final {
   public:
    PickupSystem();

    void reset();
    void update(EntityManager& entityManager, Player& player, float deltaTime);

    [[nodiscard]] std::size_t getActivePowerUpCount(
        EntityManager& entityManager) const;
    [[nodiscard]] float getNextSpawnRatio() const;

   private:
    static constexpr float kSpawnInterval = 6.0F;
    static constexpr std::size_t kMaxActivePowerUps = 3U;
    static constexpr int kHealAmount = 25;
    static constexpr float kSpeedBoostDuration = 4.0F;
    static constexpr float kSpeedBoostMultiplier = 1.45F;
    static constexpr float kRapidFireDuration = 4.5F;

    void spawnPowerUp(EntityManager& entityManager);
    void collectTouchedPowerUps(EntityManager& entityManager,
                                Player& player) const;
    void applyPowerUp(Player& player, PowerUpType type) const;

    [[nodiscard]] sf::Vector2f getNextSpawnPoint();
    [[nodiscard]] PowerUpType getNextPowerUpType();

    std::vector<sf::Vector2f> spawnPoints_;
    std::size_t nextSpawnPointIndex_ = 0;
    std::size_t nextPowerUpTypeIndex_ = 0;
    float spawnTimer_ = 2.0F;
};

}  // namespace roboarena

#endif  // ROBOARENA_SYSTEMS_PICKUP_SYSTEM_HPP
