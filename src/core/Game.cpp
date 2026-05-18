#include "core/Game.hpp"

#include <algorithm>
#include <sstream>

#include "entities/ChaserEnemy.hpp"
#include "entities/Projectile.hpp"
#include "math/VectorUtils.hpp"

namespace roboarena {

Game::Game()
    : window_(sf::VideoMode(kWindowWidth, kWindowHeight), "RoboArena"),
      map_(30, 20, 32.0F) {
    window_.setFramerateLimit(kFrameLimit);
    createEntities();
    updateWindowTitle();
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

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            const sf::Vector2i mousePixelPosition{event.mouseButton.x,
                                                  event.mouseButton.y};
            shootAt(window_.mapPixelToCoords(mousePixelPosition));
        }
    }
}

void Game::update(float deltaTime) {
    shootCooldown_ = std::max(0.0F, shootCooldown_ - deltaTime);

    if (player_ == nullptr) {
        return;
    }

    if (player_->isDefeated()) {
        updateWindowTitle();
        return;
    }

    entityManager_.updateAll(deltaTime);
    score_ += combatSystem_.update(entityManager_, *player_, deltaTime);
    entityManager_.removeDestroyed();
    updateWindowTitle();
}

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
    entityManager_.create<ChaserEnemy>(sf::Vector2f{160.0F, 520.0F}, *player_,
                                       map_);
}

void Game::shootAt(sf::Vector2f targetPosition) {
    if (player_ == nullptr || player_->isDefeated() || shootCooldown_ > 0.0F) {
        return;
    }

    const sf::Vector2f direction = targetPosition - player_->getPosition();

    if (math::length(direction) <= 0.0001F) {
        return;
    }

    entityManager_.create<Projectile>(player_->getPosition(), direction, map_);
    shootCooldown_ = kShootCooldown;
}

void Game::updateWindowTitle() {
    if (player_ == nullptr) {
        return;
    }

    std::ostringstream title;
    title << "RoboArena | HP: " << player_->getHealth() << '/'
          << player_->getMaxHealth() << " | Score: " << score_
          << " | Entities: " << entityManager_.size();

    if (player_->isDefeated()) {
        title << " | GAME OVER - press Esc";
    }

    window_.setTitle(title.str());
}

}  // namespace roboarena
