#include "core/Game.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <sstream>
#include <string>

#include "entities/Projectile.hpp"
#include "math/VectorUtils.hpp"

namespace roboarena {

Game::Game()
    : window_(sf::VideoMode(kWindowWidth, kWindowHeight), "RoboArena"),
      map_(30, 20, 32.0F) {
    window_.setFramerateLimit(kFrameLimit);
    loadFont();
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
            handleKeyPress(static_cast<sf::Keyboard::Key>(event.key.code));
        }

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            const sf::Vector2i mousePixelPosition{event.mouseButton.x,
                                                  event.mouseButton.y};
            shootAt(window_.mapPixelToCoords(mousePixelPosition));
        }
    }
}

void Game::handleKeyPress(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Escape) {
        if (state_ == GameState::Playing) {
            state_ = GameState::Paused;
            updateWindowTitle();
        } else if (state_ == GameState::Paused) {
            state_ = GameState::Playing;
            updateWindowTitle();
        } else {
            window_.close();
        }
        return;
    }

    if (key == sf::Keyboard::Return && state_ == GameState::MainMenu) {
        startGame();
        return;
    }

    if (key == sf::Keyboard::Tab && state_ == GameState::MainMenu) {
        waveSystem_.cycleDifficulty();
        restart();
        state_ = GameState::MainMenu;
        updateWindowTitle();
        return;
    }

    if (key == sf::Keyboard::P) {
        togglePause();
        return;
    }

    if (key == sf::Keyboard::R) {
        restart();
    }
}

void Game::startGame() {
    restart();
    state_ = GameState::Playing;
    updateWindowTitle();
}

void Game::togglePause() {
    if (state_ == GameState::Playing) {
        state_ = GameState::Paused;
    } else if (state_ == GameState::Paused) {
        state_ = GameState::Playing;
    }

    updateWindowTitle();
}

void Game::update(float deltaTime) {
    if (player_ == nullptr) {
        return;
    }

    if (!isPlaying()) {
        updateWindowTitle();
        return;
    }

    shootCooldown_ = std::max(0.0F, shootCooldown_ - deltaTime);

    waveSystem_.update(entityManager_, *player_, map_, deltaTime);
    entityManager_.updateAll(deltaTime);
    pickupSystem_.update(entityManager_, *player_, deltaTime);
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

    if (state_ == GameState::MainMenu) {
        drawMainMenu();
    } else if (state_ == GameState::Paused) {
        drawPauseOverlay();
    } else if (state_ == GameState::Victory || state_ == GameState::GameOver) {
        drawEndOverlay();
    }

    window_.display();
}

void Game::restart() {
    entityManager_.clear();
    combatSystem_.reset();
    waveSystem_.reset();
    pickupSystem_.reset();
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
    shootCooldown_ = kShootCooldown * player_->getShootCooldownMultiplier();
}

void Game::updateWindowTitle() {
    if (player_ == nullptr) {
        return;
    }

    std::ostringstream title;
    title << "RoboArena | Difficulty: " << waveSystem_.getDifficultyName()
          << " | HP: " << player_->getHealth() << '/' << player_->getMaxHealth()
          << " | Score: " << score_
          << " | Wave: " << waveSystem_.getCurrentWaveNumber() << '/'
          << waveSystem_.getTotalWaves()
          << " | Spawned: " << waveSystem_.getSpawnedInCurrentWave() << '/'
          << waveSystem_.getEnemiesInCurrentWave()
          << " | Enemies: " << waveSystem_.getAliveEnemies(entityManager_)
          << " | PowerUps: "
          << pickupSystem_.getActivePowerUpCount(entityManager_)
          << " | Entities: " << entityManager_.size();

    if (state_ == GameState::MainMenu) {
        title << " | MENU - Enter start, Tab difficulty, Esc exit";
    }

    if (state_ == GameState::Paused) {
        title << " | PAUSED - P/Esc resume, R restart";
    }

    if (state_ == GameState::GameOver) {
        title << " | GAME OVER - R restart or Esc exit";
    }

    if (state_ == GameState::Victory) {
        title << " | VICTORY - R restart or Esc exit";
    }

    window_.setTitle(title.str());
}

void Game::loadFont() {
    const std::array<std::string, 7> fontPaths{
        "assets/fonts/DejaVuSans.ttf",
        "assets/fonts/arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
        "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf",
        "/usr/share/fonts/truetype/freefont/FreeSans.ttf",
        "C:/Windows/Fonts/arial.ttf"};

    for (const std::string& path : fontPaths) {
        if (font_.loadFromFile(path)) {
            hasFont_ = true;
            return;
        }
    }

    hasFont_ = false;
}

void Game::drawHud() {
    if (player_ == nullptr) {
        return;
    }

    sf::RectangleShape panel({430.0F, 166.0F});
    panel.setPosition(10.0F, 10.0F);
    panel.setFillColor(sf::Color(8, 10, 16, 205));
    panel.setOutlineColor(sf::Color(110, 125, 155));
    panel.setOutlineThickness(1.0F);
    window_.draw(panel);

    const float healthRatio = static_cast<float>(player_->getHealth()) /
                              static_cast<float>(player_->getMaxHealth());
    const float shootReadyRatio =
        std::clamp(1.0F - shootCooldown_ / kShootCooldown, 0.0F, 1.0F);

    drawText("HP", {24.0F, 20.0F}, 14U, sf::Color::White);
    drawText(std::to_string(player_->getHealth()) + "/" +
                 std::to_string(player_->getMaxHealth()),
             {350.0F, 20.0F}, 14U, sf::Color::White);
    drawBar({92.0F, 23.0F}, {230.0F, 11.0F}, healthRatio,
            sf::Color(80, 220, 105));

    drawText("WAVE", {24.0F, 45.0F}, 14U, sf::Color::White);
    drawText(std::to_string(waveSystem_.getCurrentWaveNumber()) + "/" +
                 std::to_string(waveSystem_.getTotalWaves()),
             {350.0F, 45.0F}, 14U, sf::Color::White);
    drawBar({92.0F, 48.0F}, {230.0F, 11.0F}, waveSystem_.getWaveProgress(),
            sf::Color(95, 145, 255));

    drawText("SHOT", {24.0F, 70.0F}, 14U, sf::Color::White);
    drawBar({92.0F, 73.0F}, {230.0F, 9.0F}, shootReadyRatio,
            sf::Color(255, 215, 95));

    drawText("SPEED", {24.0F, 94.0F}, 14U, sf::Color::White);
    drawBar({92.0F, 97.0F}, {230.0F, 9.0F}, player_->getSpeedBoostRatio(),
            sf::Color(95, 170, 255));

    drawText("FIRE", {24.0F, 118.0F}, 14U, sf::Color::White);
    drawBar({92.0F, 121.0F}, {230.0F, 9.0F}, player_->getRapidFireRatio(),
            sf::Color(255, 130, 95));

    drawText("BONUS", {24.0F, 142.0F}, 14U, sf::Color::White);
    drawBar({92.0F, 145.0F}, {230.0F, 8.0F}, pickupSystem_.getNextSpawnRatio(),
            sf::Color(190, 120, 255));
}

void Game::drawMainMenu() {
    sf::RectangleShape overlay(
        {static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight)});
    overlay.setPosition(0.0F, 0.0F);
    overlay.setFillColor(sf::Color(10, 14, 24, 185));
    window_.draw(overlay);

    drawHelpCard({250.0F, 145.0F}, {460.0F, 315.0F}, sf::Color(95, 145, 255));
    drawText("ROBOARENA", {324.0F, 184.0F}, 28U, sf::Color(220, 235, 255));
    drawText("Enter - start game", {314.0F, 248.0F}, 16U, sf::Color::White);
    drawText(
        std::string("Tab - difficulty: ") + waveSystem_.getDifficultyName(),
        {314.0F, 280.0F}, 16U, sf::Color(255, 235, 160));
    drawText("WASD / arrows - move", {314.0F, 312.0F}, 16U, sf::Color::White);
    drawText("Left mouse - shoot", {314.0F, 344.0F}, 16U, sf::Color::White);
    drawText("P or Esc - pause", {314.0F, 376.0F}, 16U, sf::Color::White);

    drawBar({314.0F, 416.0F}, {332.0F, 12.0F},
            waveSystem_.getDifficulty() == WaveSystem::Difficulty::Easy ? 0.33F
            : waveSystem_.getDifficulty() == WaveSystem::Difficulty::Normal
                ? 0.66F
                : 1.0F,
            sf::Color(255, 215, 95));
}

void Game::drawPauseOverlay() {
    sf::RectangleShape overlay(
        {static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight)});
    overlay.setPosition(0.0F, 0.0F);
    overlay.setFillColor(sf::Color(20, 20, 28, 160));
    window_.draw(overlay);

    drawHelpCard({300.0F, 205.0F}, {360.0F, 210.0F}, sf::Color(255, 215, 95));
    drawText("PAUSED", {395.0F, 244.0F}, 24U, sf::Color(255, 235, 160));
    drawText("P or Esc - resume", {350.0F, 300.0F}, 16U, sf::Color::White);
    drawText("R - restart", {350.0F, 332.0F}, 16U, sf::Color::White);
    drawText("Esc from menu/end - exit", {350.0F, 364.0F}, 16U,
             sf::Color::White);
}

void Game::drawEndOverlay() {
    const bool victory = state_ == GameState::Victory;
    sf::RectangleShape overlay(
        {static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight)});
    overlay.setPosition(0.0F, 0.0F);
    overlay.setFillColor(victory ? sf::Color(35, 120, 70, 150)
                                 : sf::Color(140, 35, 35, 150));
    window_.draw(overlay);

    drawHelpCard({270.0F, 190.0F}, {420.0F, 245.0F},
                 victory ? sf::Color(80, 220, 105) : sf::Color(255, 110, 110));
    drawText(victory ? "VICTORY" : "GAME OVER", {350.0F, 230.0F}, 24U,
             victory ? sf::Color(150, 255, 180) : sf::Color(255, 150, 150));
    drawText("Score: " + std::to_string(score_), {350.0F, 292.0F}, 16U,
             sf::Color::White);
    drawText("R - restart", {350.0F, 326.0F}, 16U, sf::Color::White);
    drawText("Esc - exit", {350.0F, 358.0F}, 16U, sf::Color::White);
}

void Game::drawHelpCard(sf::Vector2f position, sf::Vector2f size,
                        sf::Color accentColor) {
    sf::RectangleShape card(size);
    card.setPosition(position);
    card.setFillColor(sf::Color(8, 10, 16, 230));
    card.setOutlineColor(accentColor);
    card.setOutlineThickness(2.0F);
    window_.draw(card);

    sf::RectangleShape header({size.x - 32.0F, 6.0F});
    header.setPosition(position.x + 16.0F, position.y + 20.0F);
    header.setFillColor(accentColor);
    window_.draw(header);
}

void Game::drawText(const std::string& text, sf::Vector2f position,
                    unsigned int size, sf::Color color) {
    if (!hasFont_) {
        drawPixelText(text, {position.x + 1.0F, position.y + 1.0F}, size,
                      sf::Color(0, 0, 0, 190));
        drawPixelText(text, position, size, color);
        return;
    }

    sf::Text shadow(text, font_, size);
    shadow.setPosition(position.x + 1.5F, position.y + 1.5F);
    shadow.setFillColor(sf::Color(0, 0, 0, 180));
    window_.draw(shadow);

    sf::Text label(text, font_, size);
    label.setPosition(position);
    label.setFillColor(color);
    window_.draw(label);
}

void Game::drawPixelText(const std::string& text, sf::Vector2f position,
                         unsigned int size, sf::Color color) {
    const float scale = std::max(1.0F, static_cast<float>(size) / 14.0F);
    const float advance = 6.0F * scale;
    const float lineHeight = 9.0F * scale;

    sf::Vector2f cursor = position;

    for (char character : text) {
        if (character == '\n') {
            cursor.x = position.x;
            cursor.y += lineHeight;
            continue;
        }

        if (character == ' ') {
            cursor.x += advance;
            continue;
        }

        drawPixelChar(character, cursor, scale, color);
        cursor.x += advance;
    }
}

void Game::drawPixelChar(char character, sf::Vector2f position, float scale,
                         sf::Color color) {
    using Glyph = std::array<const char*, 7>;

    const char upper =
        static_cast<char>(std::toupper(static_cast<unsigned char>(character)));
    Glyph glyph{"00000", "00000", "00000", "00000", "00000", "00000", "00000"};

    switch (upper) {
        case 'A':
            glyph = {"01110", "10001", "10001", "11111",
                     "10001", "10001", "10001"};
            break;
        case 'B':
            glyph = {"11110", "10001", "10001", "11110",
                     "10001", "10001", "11110"};
            break;
        case 'C':
            glyph = {"01111", "10000", "10000", "10000",
                     "10000", "10000", "01111"};
            break;
        case 'D':
            glyph = {"11110", "10001", "10001", "10001",
                     "10001", "10001", "11110"};
            break;
        case 'E':
            glyph = {"11111", "10000", "10000", "11110",
                     "10000", "10000", "11111"};
            break;
        case 'F':
            glyph = {"11111", "10000", "10000", "11110",
                     "10000", "10000", "10000"};
            break;
        case 'G':
            glyph = {"01111", "10000", "10000", "10111",
                     "10001", "10001", "01111"};
            break;
        case 'H':
            glyph = {"10001", "10001", "10001", "11111",
                     "10001", "10001", "10001"};
            break;
        case 'I':
            glyph = {"11111", "00100", "00100", "00100",
                     "00100", "00100", "11111"};
            break;
        case 'J':
            glyph = {"00111", "00010", "00010", "00010",
                     "10010", "10010", "01100"};
            break;
        case 'K':
            glyph = {"10001", "10010", "10100", "11000",
                     "10100", "10010", "10001"};
            break;
        case 'L':
            glyph = {"10000", "10000", "10000", "10000",
                     "10000", "10000", "11111"};
            break;
        case 'M':
            glyph = {"10001", "11011", "10101", "10101",
                     "10001", "10001", "10001"};
            break;
        case 'N':
            glyph = {"10001", "11001", "10101", "10011",
                     "10001", "10001", "10001"};
            break;
        case 'O':
            glyph = {"01110", "10001", "10001", "10001",
                     "10001", "10001", "01110"};
            break;
        case 'P':
            glyph = {"11110", "10001", "10001", "11110",
                     "10000", "10000", "10000"};
            break;
        case 'Q':
            glyph = {"01110", "10001", "10001", "10001",
                     "10101", "10010", "01101"};
            break;
        case 'R':
            glyph = {"11110", "10001", "10001", "11110",
                     "10100", "10010", "10001"};
            break;
        case 'S':
            glyph = {"01111", "10000", "10000", "01110",
                     "00001", "00001", "11110"};
            break;
        case 'T':
            glyph = {"11111", "00100", "00100", "00100",
                     "00100", "00100", "00100"};
            break;
        case 'U':
            glyph = {"10001", "10001", "10001", "10001",
                     "10001", "10001", "01110"};
            break;
        case 'V':
            glyph = {"10001", "10001", "10001", "10001",
                     "10001", "01010", "00100"};
            break;
        case 'W':
            glyph = {"10001", "10001", "10001", "10101",
                     "10101", "10101", "01010"};
            break;
        case 'X':
            glyph = {"10001", "10001", "01010", "00100",
                     "01010", "10001", "10001"};
            break;
        case 'Y':
            glyph = {"10001", "10001", "01010", "00100",
                     "00100", "00100", "00100"};
            break;
        case 'Z':
            glyph = {"11111", "00001", "00010", "00100",
                     "01000", "10000", "11111"};
            break;
        case '0':
            glyph = {"01110", "10001", "10011", "10101",
                     "11001", "10001", "01110"};
            break;
        case '1':
            glyph = {"00100", "01100", "00100", "00100",
                     "00100", "00100", "01110"};
            break;
        case '2':
            glyph = {"01110", "10001", "00001", "00010",
                     "00100", "01000", "11111"};
            break;
        case '3':
            glyph = {"11110", "00001", "00001", "01110",
                     "00001", "00001", "11110"};
            break;
        case '4':
            glyph = {"00010", "00110", "01010", "10010",
                     "11111", "00010", "00010"};
            break;
        case '5':
            glyph = {"11111", "10000", "10000", "11110",
                     "00001", "00001", "11110"};
            break;
        case '6':
            glyph = {"01110", "10000", "10000", "11110",
                     "10001", "10001", "01110"};
            break;
        case '7':
            glyph = {"11111", "00001", "00010", "00100",
                     "01000", "01000", "01000"};
            break;
        case '8':
            glyph = {"01110", "10001", "10001", "01110",
                     "10001", "10001", "01110"};
            break;
        case '9':
            glyph = {"01110", "10001", "10001", "01111",
                     "00001", "00001", "01110"};
            break;
        case '/':
            glyph = {"00001", "00001", "00010", "00100",
                     "01000", "10000", "10000"};
            break;
        case '-':
            glyph = {"00000", "00000", "00000", "11111",
                     "00000", "00000", "00000"};
            break;
        case ':':
            glyph = {"00000", "00100", "00100", "00000",
                     "00100", "00100", "00000"};
            break;
        default:
            glyph = {"11111", "10001", "00010", "00100",
                     "00100", "00000", "00100"};
            break;
    }

    sf::RectangleShape pixel({scale, scale});
    pixel.setFillColor(color);

    for (std::size_t row = 0; row < glyph.size(); ++row) {
        for (std::size_t column = 0; column < 5U; ++column) {
            if (glyph[row][column] == '1') {
                pixel.setPosition(
                    position.x + static_cast<float>(column) * scale,
                    position.y + static_cast<float>(row) * scale);
                window_.draw(pixel);
            }
        }
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
