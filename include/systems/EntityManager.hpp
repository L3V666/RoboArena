#ifndef ROBOARENA_SYSTEMS_ENTITY_MANAGER_HPP
#define ROBOARENA_SYSTEMS_ENTITY_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "entities/Entity.hpp"

namespace roboarena {

class EntityManager final {
   public:
    EntityManager() = default;

    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;
    EntityManager(EntityManager&&) = default;
    EntityManager& operator=(EntityManager&&) = default;

    template <typename EntityType, typename... Args>
    EntityType& create(Args&&... args);

    void updateAll(float deltaTime);
    void drawAll(sf::RenderWindow& window) const;
    void clear();

    [[nodiscard]] std::size_t size() const;

   private:
    std::vector<std::unique_ptr<Entity>> entities_;
};

template <typename EntityType, typename... Args>
EntityType& EntityManager::create(Args&&... args) {
    static_assert(std::is_base_of_v<Entity, EntityType>,
                  "EntityType must inherit from Entity");

    auto entity = std::make_unique<EntityType>(std::forward<Args>(args)...);
    EntityType& reference = *entity;
    entities_.push_back(std::move(entity));
    return reference;
}

}  // namespace roboarena

#endif  // ROBOARENA_SYSTEMS_ENTITY_MANAGER_HPP
