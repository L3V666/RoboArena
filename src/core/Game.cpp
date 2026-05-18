#include "core/Game.hpp"

namespace roboarena {

Game::Game()
    : window_(sf::VideoMode(kWindowWidth, kWindowHeight), "RoboArena"),
      map_(30, 20, 32.0F),
      player_({96.0F, 96.0F}) {
    window_.setFramerateLimit(kFrameLimit);
    player_.setMap(&map_);
}

void Game::run() {
    sf::Clock clock;

    while (window_.isOpen()) {
        const float deltaTime = clock.restart().asSeconds();

        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event{};

    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
        }

        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape) {
            window_.close();
        }
    }
}

void Game::update(float deltaTime) { player_.update(deltaTime); }

void Game::render() {
    window_.clear(sf::Color(18, 18, 24));
    map_.draw(window_);
    player_.draw(window_);
    window_.display();
}

}  // namespace roboarena
