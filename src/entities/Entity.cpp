#include "entities/Entity.hpp"

namespace roboarena {

Entity::~Entity() = default;

bool Entity::shouldBeRemoved() const { return false; }

}  // namespace roboarena
