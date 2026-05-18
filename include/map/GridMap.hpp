#ifndef ROBOARENA_MAP_GRID_MAP_HPP
#define ROBOARENA_MAP_GRID_MAP_HPP

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <vector>

#include "map/Tile.hpp"

namespace roboarena {

class GridMap final {
   public:
    GridMap(std::size_t width, std::size_t height, float tileSize);

    void draw(sf::RenderWindow& window) const;

    [[nodiscard]] bool isWalkablePixel(sf::Vector2f position) const;
    [[nodiscard]] bool isInsidePixel(sf::Vector2f position) const;
    [[nodiscard]] bool isWalkableCell(sf::Vector2i cell) const;
    [[nodiscard]] bool isInsideCell(sf::Vector2i cell) const;
    [[nodiscard]] sf::Vector2i worldToCell(sf::Vector2f position) const;
    [[nodiscard]] sf::Vector2f cellToWorld(sf::Vector2i cell) const;
    [[nodiscard]] float getTileSize() const;
    [[nodiscard]] std::size_t getWidth() const;
    [[nodiscard]] std::size_t getHeight() const;

   private:
    void generateDemoArena();
    [[nodiscard]] bool isInsideCell(int x, int y) const;
    [[nodiscard]] const Tile& getTile(std::size_t x, std::size_t y) const;

    std::size_t width_;
    std::size_t height_;
    float tileSize_;
    std::vector<std::vector<Tile>> tiles_;
};

}  // namespace roboarena

#endif  // ROBOARENA_MAP_GRID_MAP_HPP
