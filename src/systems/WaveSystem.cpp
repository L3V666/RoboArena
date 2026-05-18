#include "systems/WaveSystem.hpp"

#include <algorithm>

#include "entities/ChaserEnemy.hpp"
#include "entities/Enemy.hpp"

namespace roboarena {

WaveSystem::WaveSystem()
    : spawnPoints_{sf::Vector2f{816.0F, 496.0F}, sf::Vector2f{816.0F, 96.0F},
                   sf::Vector2f{160.0F, 528.0F}, sf::Vector2f{496.0F, 560.0F}} {
    configureWaves();
}

void WaveSystem::reset() {
    configureWaves();
    currentWaveIndex_ = 0;
    spawnedInCurrentWave_ = 0;
    nextSpawnPointIndex_ = 0;
    spawnTimer_ = 0.0F;
}

void WaveSystem::cycleDifficulty() {
    switch (difficulty_) {
        case Difficulty::Easy:
            setDifficulty(Difficulty::Normal);
            break;
        case Difficulty::Normal:
            setDifficulty(Difficulty::Hard);
            break;
        case Difficulty::Hard:
            setDifficulty(Difficulty::Easy);
            break;
    }
}

void WaveSystem::setDifficulty(Difficulty difficulty) {
    difficulty_ = difficulty;
    reset();
}

void WaveSystem::update(EntityManager& entityManager, const Player& player,
                        const GridMap& map, float deltaTime) {
    if (currentWaveIndex_ >= waves_.size() || player.isDefeated()) {
        return;
    }

    advanceWaveIfCleared(entityManager);

    if (currentWaveIndex_ >= waves_.size() || allEnemiesSpawned()) {
        return;
    }

    spawnTimer_ = std::max(0.0F, spawnTimer_ - deltaTime);

    if (spawnTimer_ > 0.0F) {
        return;
    }

    spawnEnemy(entityManager, player, map);
    ++spawnedInCurrentWave_;
    spawnTimer_ = waves_[currentWaveIndex_].spawnInterval;
}

bool WaveSystem::isFinished(EntityManager& entityManager) const {
    return currentWaveIndex_ >= waves_.size() &&
           getAliveEnemies(entityManager) == 0U;
}

std::size_t WaveSystem::getCurrentWaveNumber() const {
    if (waves_.empty()) {
        return 0U;
    }

    return std::min(currentWaveIndex_ + 1U, waves_.size());
}

std::size_t WaveSystem::getTotalWaves() const { return waves_.size(); }

std::size_t WaveSystem::getSpawnedInCurrentWave() const {
    return spawnedInCurrentWave_;
}

std::size_t WaveSystem::getEnemiesInCurrentWave() const {
    if (currentWaveIndex_ >= waves_.size()) {
        return 0U;
    }

    return waves_[currentWaveIndex_].enemyCount;
}

std::size_t WaveSystem::getAliveEnemies(EntityManager& entityManager) const {
    const std::vector<Enemy*> enemies = entityManager.getAll<Enemy>();
    std::size_t aliveEnemies = 0;

    for (const Enemy* enemy : enemies) {
        if (!enemy->shouldBeRemoved()) {
            ++aliveEnemies;
        }
    }

    return aliveEnemies;
}

float WaveSystem::getWaveProgress() const {
    const std::size_t enemyCount = getEnemiesInCurrentWave();

    if (enemyCount == 0U) {
        return 1.0F;
    }

    return static_cast<float>(spawnedInCurrentWave_) /
           static_cast<float>(enemyCount);
}

WaveSystem::Difficulty WaveSystem::getDifficulty() const { return difficulty_; }

const char* WaveSystem::getDifficultyName() const {
    switch (difficulty_) {
        case Difficulty::Easy:
            return "Easy";
        case Difficulty::Normal:
            return "Normal";
        case Difficulty::Hard:
            return "Hard";
    }

    return "Unknown";
}

void WaveSystem::configureWaves() {
    switch (difficulty_) {
        case Difficulty::Easy:
            waves_ = {WaveConfig{3U, 1.25F}, WaveConfig{4U, 1.05F},
                      WaveConfig{5U, 0.90F}, WaveConfig{7U, 0.75F}};
            break;
        case Difficulty::Normal:
            waves_ = {WaveConfig{3U, 1.10F}, WaveConfig{5U, 0.90F},
                      WaveConfig{7U, 0.75F}, WaveConfig{10U, 0.60F}};
            break;
        case Difficulty::Hard:
            waves_ = {WaveConfig{4U, 0.85F}, WaveConfig{7U, 0.70F},
                      WaveConfig{10U, 0.55F}, WaveConfig{14U, 0.45F}};
            break;
    }
}

void WaveSystem::spawnEnemy(EntityManager& entityManager, const Player& player,
                            const GridMap& map) {
    entityManager.create<ChaserEnemy>(getNextSpawnPoint(), player, map);
}

void WaveSystem::advanceWaveIfCleared(EntityManager& entityManager) {
    if (!allEnemiesSpawned()) {
        return;
    }

    if (getAliveEnemies(entityManager) != 0U) {
        return;
    }

    ++currentWaveIndex_;
    spawnedInCurrentWave_ = 0;
    spawnTimer_ = kTimeBetweenWaves;
}

sf::Vector2f WaveSystem::getNextSpawnPoint() {
    const sf::Vector2f spawnPoint = spawnPoints_[nextSpawnPointIndex_];
    nextSpawnPointIndex_ = (nextSpawnPointIndex_ + 1U) % spawnPoints_.size();
    return spawnPoint;
}

bool WaveSystem::allEnemiesSpawned() const {
    return currentWaveIndex_ >= waves_.size() ||
           spawnedInCurrentWave_ >= waves_[currentWaveIndex_].enemyCount;
}

}  // namespace roboarena
