#ifndef ROBOARENA_SYSTEMS_PATHFINDER_HPP
#define ROBOARENA_SYSTEMS_PATHFINDER_HPP

#include <SFML/System/Vector2.hpp>
#include <cstddef>
#include <functional>
#include <unordered_map>
#include <vector>

#include "map/GridMap.hpp"

namespace roboarena {

struct CellHash final {
    [[nodiscard]] std::size_t operator()(sf::Vector2i cell) const noexcept;
};

class Pathfinder final {
   public:
    explicit Pathfinder(const GridMap& map);

    [[nodiscard]] std::vector<sf::Vector2i> findPath(sf::Vector2i start,
                                                     sf::Vector2i goal) const;

   private:
    struct QueueNode final {
        sf::Vector2i cell{};
        int priority = 0;
    };

    struct QueueNodeComparator final {
        [[nodiscard]] bool operator()(const QueueNode& left,
                                      const QueueNode& right) const;
    };

    [[nodiscard]] std::vector<sf::Vector2i> getNeighbors(
        sf::Vector2i cell) const;
    [[nodiscard]] int heuristic(sf::Vector2i from, sf::Vector2i to) const;
    [[nodiscard]] std::vector<sf::Vector2i> restorePath(
        sf::Vector2i start, sf::Vector2i goal,
        const std::unordered_map<sf::Vector2i, sf::Vector2i, CellHash>&
            cameFrom) const;

    const GridMap& map_;
};

}  // namespace roboarena

#endif  // ROBOARENA_SYSTEMS_PATHFINDER_HPP
