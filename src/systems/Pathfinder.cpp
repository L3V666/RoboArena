#include "systems/Pathfinder.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <queue>
#include <unordered_map>

namespace roboarena {

std::size_t CellHash::operator()(sf::Vector2i cell) const noexcept {
    const auto x = static_cast<std::size_t>(cell.x);
    const auto y = static_cast<std::size_t>(cell.y);
    return x * 73856093U ^ y * 19349663U;
}

Pathfinder::Pathfinder(const GridMap& map) : map_(map) {}

std::vector<sf::Vector2i> Pathfinder::findPath(sf::Vector2i start,
                                               sf::Vector2i goal) const {
    if (!map_.isWalkableCell(start) || !map_.isWalkableCell(goal)) {
        return {};
    }

    std::priority_queue<QueueNode, std::vector<QueueNode>, QueueNodeComparator>
        frontier;
    frontier.push(QueueNode{start, 0});

    std::unordered_map<sf::Vector2i, sf::Vector2i, CellHash> cameFrom;
    std::unordered_map<sf::Vector2i, int, CellHash> costSoFar;

    cameFrom.emplace(start, start);
    costSoFar.emplace(start, 0);

    while (!frontier.empty()) {
        const sf::Vector2i current = frontier.top().cell;
        frontier.pop();

        if (current == goal) {
            break;
        }

        for (const sf::Vector2i next : getNeighbors(current)) {
            const int newCost = costSoFar.at(current) + 1;
            const auto knownCost = costSoFar.find(next);

            if (knownCost == costSoFar.end() || newCost < knownCost->second) {
                costSoFar[next] = newCost;
                const int priority = newCost + heuristic(next, goal);
                frontier.push(QueueNode{next, priority});
                cameFrom[next] = current;
            }
        }
    }

    if (cameFrom.find(goal) == cameFrom.end()) {
        return {};
    }

    return restorePath(start, goal, cameFrom);
}

bool Pathfinder::QueueNodeComparator::operator()(const QueueNode& left,
                                                 const QueueNode& right) const {
    return left.priority > right.priority;
}

std::vector<sf::Vector2i> Pathfinder::getNeighbors(sf::Vector2i cell) const {
    static const std::array<sf::Vector2i, 4> kDirections{
        sf::Vector2i{1, 0}, sf::Vector2i{-1, 0}, sf::Vector2i{0, 1},
        sf::Vector2i{0, -1}};

    std::vector<sf::Vector2i> neighbors;
    neighbors.reserve(kDirections.size());

    for (const sf::Vector2i direction : kDirections) {
        const sf::Vector2i next = cell + direction;

        if (map_.isWalkableCell(next)) {
            neighbors.push_back(next);
        }
    }

    return neighbors;
}

int Pathfinder::heuristic(sf::Vector2i from, sf::Vector2i to) const {
    return std::abs(from.x - to.x) + std::abs(from.y - to.y);
}

std::vector<sf::Vector2i> Pathfinder::restorePath(
    sf::Vector2i start, sf::Vector2i goal,
    const std::unordered_map<sf::Vector2i, sf::Vector2i, CellHash>& cameFrom)
    const {
    std::vector<sf::Vector2i> path;
    sf::Vector2i current = goal;

    path.push_back(current);

    while (current != start) {
        current = cameFrom.at(current);
        path.push_back(current);
    }

    std::reverse(path.begin(), path.end());
    return path;
}

}  // namespace roboarena
