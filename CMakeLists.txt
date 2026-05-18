cmake_minimum_required(VERSION 3.16)

project(RoboArena VERSION 0.7.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_library(roboarena_math
    src/math/VectorUtils.cpp
)

target_include_directories(roboarena_math PUBLIC include)

add_library(roboarena_map
    src/map/GridMap.cpp
)

target_include_directories(roboarena_map PUBLIC include)
target_link_libraries(roboarena_map PUBLIC sfml-graphics)

add_library(roboarena_pathfinding
    src/systems/Pathfinder.cpp
)

target_include_directories(roboarena_pathfinding PUBLIC include)
target_link_libraries(roboarena_pathfinding PUBLIC roboarena_map)

add_library(roboarena_entities
    src/entities/Entity.cpp
    src/entities/Player.cpp
    src/entities/Enemy.cpp
    src/entities/ChaserEnemy.cpp
    src/entities/Projectile.cpp
    src/entities/PowerUp.cpp
)

target_include_directories(roboarena_entities PUBLIC include)
target_link_libraries(roboarena_entities PUBLIC sfml-graphics roboarena_math roboarena_map roboarena_pathfinding)

add_library(roboarena_systems
    src/systems/EntityManager.cpp
    src/systems/CombatSystem.cpp
    src/systems/WaveSystem.cpp
    src/systems/PickupSystem.cpp
)

target_include_directories(roboarena_systems PUBLIC include)
target_link_libraries(roboarena_systems PUBLIC sfml-graphics roboarena_entities)

add_library(roboarena_core
    src/core/Game.cpp
)

target_include_directories(roboarena_core PUBLIC include)
target_link_libraries(roboarena_core PUBLIC sfml-graphics sfml-window sfml-system roboarena_entities roboarena_map roboarena_systems)

add_executable(RoboArena
    src/main.cpp
)

target_link_libraries(RoboArena PRIVATE roboarena_core)
