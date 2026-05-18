#include "systems/CombatSystem.hpp"

#include <algorithm>
#include <vector>

#include "entities/Enemy.hpp"
#include "entities/Projectile.hpp"

namespace roboarena {

std::size_t CombatSystem::update(EntityManager& entityManager, Player& player,
                                 float deltaTime) {
    const std::size_t killedEnemies = handleProjectileHits(entityManager);
    handlePlayerContactDamage(entityManager, player, deltaTime);
    return killedEnemies;
}

std::size_t CombatSystem::handleProjectileHits(
    EntityManager& entityManager) const {
    const std::vector<Projectile*> projectiles =
        entityManager.getAll<Projectile>();
    const std::vector<Enemy*> enemies = entityManager.getAll<Enemy>();

    std::size_t killedEnemies = 0;

    for (Projectile* projectile : projectiles) {
        if (projectile->shouldBeRemoved()) {
            continue;
        }

        for (Enemy* enemy : enemies) {
            if (enemy->shouldBeRemoved()) {
                continue;
            }

            if (!projectile->getBounds().intersects(enemy->getBounds())) {
                continue;
            }

            enemy->takeDamage(projectile->getDamage());
            projectile->destroy();

            if (enemy->shouldBeRemoved()) {
                ++killedEnemies;
            }

            break;
        }
    }

    return killedEnemies;
}

void CombatSystem::handlePlayerContactDamage(EntityManager& entityManager,
                                             Player& player, float deltaTime) {
    playerHitTimer_ = std::max(0.0F, playerHitTimer_ - deltaTime);

    if (playerHitTimer_ > 0.0F || player.isDefeated()) {
        return;
    }

    const std::vector<Enemy*> enemies = entityManager.getAll<Enemy>();

    for (const Enemy* enemy : enemies) {
        if (enemy->shouldBeRemoved()) {
            continue;
        }

        if (!enemy->getBounds().intersects(player.getBounds())) {
            continue;
        }

        player.takeDamage(enemy->getContactDamage());
        playerHitTimer_ = kPlayerHitCooldown;
        return;
    }
}

}  // namespace roboarena
