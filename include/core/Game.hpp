#ifndef ROBOARENA_CORE_GAME_HPP
#define ROBOARENA_CORE_GAME_HPP

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <string>

#include "entities/Player.hpp"
#include "map/GridMap.hpp"
#include "systems/CombatSystem.hpp"
#include "systems/EntityManager.hpp"
#include "systems/PickupSystem.hpp"
#include "systems/WaveSystem.hpp"

namespace roboarena {

class Game final {
   public:
    Game();

    void run();

   private:
    enum class GameState { MainMenu, Playing, Paused, Victory, GameOver };

    static constexpr unsigned int kWindowWidth = 960;
    static constexpr unsigned int kWindowHeight = 640;
    static constexpr unsigned int kFrameLimit = 120;
    static constexpr float kShootCooldown = 0.18F;

    void processEvents();
    void handleKeyPress(sf::Keyboard::Key key);
    void startGame();
    void togglePause();
    void update(float deltaTime);
    void render();
    void restart();
    void createPlayer();
    void shootAt(sf::Vector2f targetPosition);
    void updateWindowTitle();
    void loadFont();
    void drawHud();
    void drawMainMenu();
    void drawPauseOverlay();
    void drawEndOverlay();
    void drawHelpCard(sf::Vector2f position, sf::Vector2f size,
                      sf::Color accentColor);
    void drawText(const std::string& text, sf::Vector2f position,
                  unsigned int size, sf::Color color);
    void drawPixelText(const std::string& text, sf::Vector2f position,
                       unsigned int size, sf::Color color);
    void drawPixelChar(char character, sf::Vector2f position, float scale,
                       sf::Color color);
    void drawBar(sf::Vector2f position, sf::Vector2f size, float ratio,
                 sf::Color fillColor);
    [[nodiscard]] bool isPlaying() const;

    sf::RenderWindow window_;
    GridMap map_;
    EntityManager entityManager_;
    CombatSystem combatSystem_;
    WaveSystem waveSystem_;
    PickupSystem pickupSystem_;
    Player* player_ = nullptr;
    std::size_t score_ = 0;
    float shootCooldown_ = 0.0F;
    GameState state_ = GameState::MainMenu;
    sf::Font font_;
    bool hasFont_ = false;
};

}  // namespace roboarena

#endif  // ROBOARENA_CORE_GAME_HPP
