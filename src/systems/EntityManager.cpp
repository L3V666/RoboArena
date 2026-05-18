#include "systems/EntityManager.hpp"

namespace roboarena {

void EntityManager::updateAll(float deltaTime) {
    for (const auto& entity : entities_) {
        entity->update(deltaTime);
    }
}

void EntityManager::drawAll(sf::RenderWindow& window) const {
    for (const auto& entity : entities_) {
        entity->draw(window);
    }
}

void EntityManager::clear() { entities_.clear(); }

std::size_t EntityManager::size() const { return entities_.size(); }

}  // namespace roboarena
