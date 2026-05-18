#include "systems/EntityManager.hpp"

#include <algorithm>

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

void EntityManager::removeDestroyed() {
    const auto newEnd =
        std::remove_if(entities_.begin(), entities_.end(),
                       [](const std::unique_ptr<Entity>& entity) {
                           return entity->shouldBeRemoved();
                       });

    entities_.erase(newEnd, entities_.end());
}

void EntityManager::clear() { entities_.clear(); }

std::size_t EntityManager::size() const { return entities_.size(); }

}  // namespace roboarena
