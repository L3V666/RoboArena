#include "core/Game.hpp"

#include "entities/ChaserEnemy.hpp"

namespace roboarena {

Game::Game()
    : window_(sf::VideoMode(kWindowWidth, kWindowHeight), "RoboArena"),
      map_(30, 20, 32.0F) {
    window_.setFramerateLimit(kFrameLimit);
    createEntities();
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

void Game::update(float deltaTime) { entityManager_.updateAll(deltaTime); }

void Game::render() {
    window_.clear(sf::Color(18, 18, 24));
    map_.draw(window_);
    entityManager_.drawAll(window_);
    window_.display();
}

void Game::createEntities() {
    Player& player = entityManager_.create<Player>(sf::Vector2f{96.0F, 96.0F});
    player.setMap(&map_);
    player_ = &player;

    entityManager_.create<ChaserEnemy>(sf::Vector2f{800.0F, 500.0F}, *player_,
                                       map_);
    entityManager_.create<ChaserEnemy>(sf::Vector2f{800.0F, 96.0F}, *player_,
                                       map_);
}

}  // namespace roboarena
