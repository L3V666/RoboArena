#ifndef ROBOARENA_CORE_GAME_HPP
#define ROBOARENA_CORE_GAME_HPP

#include <SFML/Graphics.hpp>

#include "entities/Player.hpp"
#include "map/GridMap.hpp"

namespace roboarena {

class Game final {
   public:
    Game();

    void run();

   private:
    static constexpr unsigned int kWindowWidth = 960;
    static constexpr unsigned int kWindowHeight = 640;
    static constexpr unsigned int kFrameLimit = 120;

    void processEvents();
    void update(float deltaTime);
    void render();

    sf::RenderWindow window_;
    GridMap map_;
    Player player_;
};

}  // namespace roboarena

#endif  // ROBOARENA_CORE_GAME_HPP
