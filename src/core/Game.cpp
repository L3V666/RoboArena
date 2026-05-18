#include "core/Game.hpp"

#include <algorithm>
#include <sstream>

#include "entities/Projectile.hpp"
#include "math/VectorUtils.hpp"

namespace roboarena {

Game::Game()
    : window_(sf::VideoMode(kWindowWidth, kWindowHeight), "RoboArena"),
      map_(30, 20, 32.0F) {
    window_.setFramerateLimit(kFrameLimit);
    createPlayer();
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

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                window_.close();
            }

            if (event.key.code == sf::Keyboard::R) {
                restart();
            }
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

    if (!isPlaying()) {
        updateWindowTitle();
        return;
    }

    waveSystem_.update(entityManager_, *player_, map_, deltaTime);
    entityManager_.updateAll(deltaTime);
    score_ += combatSystem_.update(entityManager_, *player_, deltaTime);
    entityManager_.removeDestroyed();

    if (player_->isDefeated()) {
        state_ = GameState::GameOver;
    } else if (waveSystem_.isFinished(entityManager_)) {
        state_ = GameState::Victory;
    }

    updateWindowTitle();
}

void Game::render() {
    window_.clear(sf::Color(18, 18, 24));
    map_.draw(window_);
    entityManager_.drawAll(window_);
    drawHud();
    window_.display();
}

void Game::restart() {
    entityManager_.clear();
    combatSystem_.reset();
    waveSystem_.reset();
    player_ = nullptr;
    score_ = 0;
    shootCooldown_ = 0.0F;
    state_ = GameState::Playing;
    createPlayer();
    updateWindowTitle();
}

void Game::createPlayer() {
    Player& player = entityManager_.create<Player>(sf::Vector2f{96.0F, 96.0F});
    player.setMap(&map_);
    player_ = &player;
}

void Game::shootAt(sf::Vector2f targetPosition) {
    if (player_ == nullptr || !isPlaying() || player_->isDefeated() ||
        shootCooldown_ > 0.0F) {
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
          << " | Wave: " << waveSystem_.getCurrentWaveNumber() << '/'
          << waveSystem_.getTotalWaves()
          << " | Spawned: " << waveSystem_.getSpawnedInCurrentWave() << '/'
          << waveSystem_.getEnemiesInCurrentWave()
          << " | Enemies: " << waveSystem_.getAliveEnemies(entityManager_)
          << " | Entities: " << entityManager_.size();

    if (state_ == GameState::GameOver) {
        title << " | GAME OVER - press R to restart or Esc to exit";
    }

    if (state_ == GameState::Victory) {
        title << " | VICTORY - press R to restart or Esc to exit";
    }

    window_.setTitle(title.str());
}

void Game::drawHud() {
    if (player_ == nullptr) {
        return;
    }

    sf::RectangleShape panel({360.0F, 70.0F});
    panel.setPosition(10.0F, 10.0F);
    panel.setFillColor(sf::Color(8, 10, 16, 190));
    panel.setOutlineColor(sf::Color(110, 125, 155));
    panel.setOutlineThickness(1.0F);
    window_.draw(panel);

    const float healthRatio = static_cast<float>(player_->getHealth()) /
                              static_cast<float>(player_->getMaxHealth());
    drawBar({24.0F, 25.0F}, {320.0F, 14.0F}, healthRatio,
            sf::Color(80, 220, 105));
    drawBar({24.0F, 48.0F}, {320.0F, 10.0F}, waveSystem_.getWaveProgress(),
            sf::Color(95, 145, 255));

    const float shootReadyRatio = 1.0F - shootCooldown_ / kShootCooldown;
    drawBar({24.0F, 64.0F}, {320.0F, 6.0F}, shootReadyRatio,
            sf::Color(255, 215, 95));

    if (!isPlaying()) {
        sf::RectangleShape overlay({static_cast<float>(kWindowWidth),
                                    static_cast<float>(kWindowHeight)});
        overlay.setPosition(0.0F, 0.0F);
        overlay.setFillColor(state_ == GameState::Victory
                                 ? sf::Color(40, 120, 80, 85)
                                 : sf::Color(140, 35, 35, 85));
        window_.draw(overlay);
    }
}

void Game::drawBar(sf::Vector2f position, sf::Vector2f size, float ratio,
                   sf::Color fillColor) {
    const float clampedRatio = std::clamp(ratio, 0.0F, 1.0F);

    sf::RectangleShape background(size);
    background.setPosition(position);
    background.setFillColor(sf::Color(35, 38, 48));
    background.setOutlineColor(sf::Color(95, 105, 125));
    background.setOutlineThickness(1.0F);
    window_.draw(background);

    sf::RectangleShape foreground({size.x * clampedRatio, size.y});
    foreground.setPosition(position);
    foreground.setFillColor(fillColor);
    window_.draw(foreground);
}

bool Game::isPlaying() const { return state_ == GameState::Playing; }

}  // namespace roboarena
