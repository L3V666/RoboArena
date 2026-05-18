#ifndef ROBOARENA_CORE_GAME_HPP
#define ROBOARENA_CORE_GAME_HPP

#include <SFML/Graphics.hpp>
#include <cstddef>

#include "entities/Player.hpp"
#include "map/GridMap.hpp"
#include "systems/CombatSystem.hpp"
#include "systems/EntityManager.hpp"
#include "systems/WaveSystem.hpp"

namespace roboarena {

class Game final {
   public:
    Game();

    void run();

   private:
    enum class GameState { Playing, Victory, GameOver };

    static constexpr unsigned int kWindowWidth = 960;
    static constexpr unsigned int kWindowHeight = 640;
    static constexpr unsigned int kFrameLimit = 120;
    static constexpr float kShootCooldown = 0.18F;

    void processEvents();
    void update(float deltaTime);
    void render();
    void restart();
    void createPlayer();
    void shootAt(sf::Vector2f targetPosition);
    void updateWindowTitle();
    void drawHud();
    void drawBar(sf::Vector2f position, sf::Vector2f size, float ratio,
                 sf::Color fillColor);
    [[nodiscard]] bool isPlaying() const;

    sf::RenderWindow window_;
    GridMap map_;
    EntityManager entityManager_;
    CombatSystem combatSystem_;
    WaveSystem waveSystem_;
    Player* player_ = nullptr;
    std::size_t score_ = 0;
    float shootCooldown_ = 0.0F;
    GameState state_ = GameState::Playing;
};

}  // namespace roboarena

#endif  // ROBOARENA_CORE_GAME_HPP
