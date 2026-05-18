#ifndef ROBOARENA_CORE_GAME_HPP
#define ROBOARENA_CORE_GAME_HPP

#include <SFML/Graphics.hpp>
#include <cstddef>

#include "entities/Player.hpp"
#include "map/GridMap.hpp"
#include "systems/CombatSystem.hpp"
#include "systems/EntityManager.hpp"

namespace roboarena {

class Game final {
   public:
    Game();

    void run();

   private:
    static constexpr unsigned int kWindowWidth = 960;
    static constexpr unsigned int kWindowHeight = 640;
    static constexpr unsigned int kFrameLimit = 120;
    static constexpr float kShootCooldown = 0.18F;

    void processEvents();
    void update(float deltaTime);
    void render();
    void createEntities();
    void shootAt(sf::Vector2f targetPosition);
    void updateWindowTitle();

    sf::RenderWindow window_;
    GridMap map_;
    EntityManager entityManager_;
    CombatSystem combatSystem_;
    Player* player_ = nullptr;
    std::size_t score_ = 0;
    float shootCooldown_ = 0.0F;
};

}  // namespace roboarena

#endif  // ROBOARENA_CORE_GAME_HPP
