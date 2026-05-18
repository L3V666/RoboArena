#ifndef ROBOARENA_MAP_TILE_HPP
#define ROBOARENA_MAP_TILE_HPP

#include <SFML/Graphics.hpp>

namespace roboarena {

enum class TileType { Floor, Wall };

struct Tile final {
    TileType type = TileType::Floor;

    [[nodiscard]] bool isWalkable() const;
    [[nodiscard]] sf::Color getColor() const;
};

}  // namespace roboarena

#endif  // ROBOARENA_MAP_TILE_HPP
