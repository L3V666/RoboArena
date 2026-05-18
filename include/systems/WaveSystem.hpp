#ifndef ROBOARENA_SYSTEMS_WAVE_SYSTEM_HPP
#define ROBOARENA_SYSTEMS_WAVE_SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <vector>

#include "entities/Player.hpp"
#include "map/GridMap.hpp"
#include "systems/EntityManager.hpp"

namespace roboarena {

class WaveSystem final {
   public:
    enum class Difficulty { Easy, Normal, Hard };

    WaveSystem();

    void reset();
    void cycleDifficulty();
    void setDifficulty(Difficulty difficulty);
    void update(EntityManager& entityManager, const Player& player,
                const GridMap& map, float deltaTime);

    [[nodiscard]] bool isFinished(EntityManager& entityManager) const;
    [[nodiscard]] std::size_t getCurrentWaveNumber() const;
    [[nodiscard]] std::size_t getTotalWaves() const;
    [[nodiscard]] std::size_t getSpawnedInCurrentWave() const;
    [[nodiscard]] std::size_t getEnemiesInCurrentWave() const;
    [[nodiscard]] std::size_t getAliveEnemies(
        EntityManager& entityManager) const;
    [[nodiscard]] float getWaveProgress() const;
    [[nodiscard]] Difficulty getDifficulty() const;
    [[nodiscard]] const char* getDifficultyName() const;

   private:
    struct WaveConfig final {
        std::size_t enemyCount;
        float spawnInterval;
    };

    static constexpr float kTimeBetweenWaves = 1.25F;

    void configureWaves();
    void spawnEnemy(EntityManager& entityManager, const Player& player,
                    const GridMap& map);
    void advanceWaveIfCleared(EntityManager& entityManager);
    [[nodiscard]] sf::Vector2f getNextSpawnPoint();
    [[nodiscard]] bool allEnemiesSpawned() const;

    std::vector<WaveConfig> waves_;
    std::vector<sf::Vector2f> spawnPoints_;
    Difficulty difficulty_ = Difficulty::Normal;
    std::size_t currentWaveIndex_ = 0;
    std::size_t spawnedInCurrentWave_ = 0;
    std::size_t nextSpawnPointIndex_ = 0;
    float spawnTimer_ = 0.0F;
};

}  // namespace roboarena

#endif  // ROBOARENA_SYSTEMS_WAVE_SYSTEM_HPP
