#include "map/GridMap.hpp"

#include <cmath>

namespace roboarena {

bool Tile::isWalkable() const { return type == TileType::Floor; }

sf::Color Tile::getColor() const {
    if (type == TileType::Wall) {
        return sf::Color(72, 78, 92);
    }

    return sf::Color(35, 38, 48);
}

GridMap::GridMap(std::size_t width, std::size_t height, float tileSize)
    : width_(width),
      height_(height),
      tileSize_(tileSize),
      tiles_(height, std::vector<Tile>(width)) {
    generateDemoArena();
}

void GridMap::draw(sf::RenderWindow& window) const {
    sf::RectangleShape rectangle({tileSize_ - 1.0F, tileSize_ - 1.0F});

    for (std::size_t y = 0; y < height_; ++y) {
        for (std::size_t x = 0; x < width_; ++x) {
            rectangle.setPosition(static_cast<float>(x) * tileSize_,
                                  static_cast<float>(y) * tileSize_);
            rectangle.setFillColor(getTile(x, y).getColor());
            window.draw(rectangle);
        }
    }
}

bool GridMap::isWalkablePixel(sf::Vector2f position) const {
    if (!isInsidePixel(position)) {
        return false;
    }

    const auto cellX =
        static_cast<std::size_t>(std::floor(position.x / tileSize_));
    const auto cellY =
        static_cast<std::size_t>(std::floor(position.y / tileSize_));

    return getTile(cellX, cellY).isWalkable();
}

bool GridMap::isInsidePixel(sf::Vector2f position) const {
    return position.x >= 0.0F && position.y >= 0.0F &&
           position.x < static_cast<float>(width_) * tileSize_ &&
           position.y < static_cast<float>(height_) * tileSize_;
}

float GridMap::getTileSize() const { return tileSize_; }

std::size_t GridMap::getWidth() const { return width_; }

std::size_t GridMap::getHeight() const { return height_; }

void GridMap::generateDemoArena() {
    for (std::size_t y = 0; y < height_; ++y) {
        for (std::size_t x = 0; x < width_; ++x) {
            const bool isBorder =
                x == 0 || y == 0 || x + 1 == width_ || y + 1 == height_;
            tiles_[y][x].type = isBorder ? TileType::Wall : TileType::Floor;
        }
    }

    for (std::size_t x = 5; x < 15; ++x) {
        tiles_[7][x].type = TileType::Wall;
    }

    for (std::size_t y = 10; y < 17; ++y) {
        tiles_[y][20].type = TileType::Wall;
    }

    for (std::size_t x = 20; x < 27; ++x) {
        tiles_[14][x].type = TileType::Wall;
    }
}

bool GridMap::isInsideCell(int x, int y) const {
    return x >= 0 && y >= 0 && x < static_cast<int>(width_) &&
           y < static_cast<int>(height_);
}

const Tile& GridMap::getTile(std::size_t x, std::size_t y) const {
    return tiles_[y][x];
}

}  // namespace roboarena
