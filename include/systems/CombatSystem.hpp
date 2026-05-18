#ifndef ROBOARENA_SYSTEMS_COMBAT_SYSTEM_HPP
#define ROBOARENA_SYSTEMS_COMBAT_SYSTEM_HPP

#include <cstddef>

#include "entities/Player.hpp"
#include "systems/EntityManager.hpp"

namespace roboarena {

class CombatSystem final {
   public:
    void reset();
    [[nodiscard]] std::size_t update(EntityManager& entityManager,
                                     Player& player, float deltaTime);

   private:
    static constexpr float kPlayerHitCooldown = 0.75F;

    [[nodiscard]] std::size_t handleProjectileHits(
        EntityManager& entityManager) const;
    void handlePlayerContactDamage(EntityManager& entityManager, Player& player,
                                   float deltaTime);

    float playerHitTimer_ = 0.0F;
};

}  // namespace roboarena

#endif  // ROBOARENA_SYSTEMS_COMBAT_SYSTEM_HPP
