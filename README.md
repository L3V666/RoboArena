# RoboArena

RoboArena — 2D-игра на C++ и SFML. Игрок управляет роботом на арене, а враги преследуют его. Начиная с третьего этапа враги не идут напрямую через стены, а строят путь по клеточной карте с помощью A*.

## Технологии

- C++17
- SFML
- CMake
- STL containers
- ООП: инкапсуляция, наследование, виртуальные методы
- A* pathfinding

## Сборка

```bash
mkdir build
cd build
cmake ..
cmake --build .
./RoboArena
```

На Windows исполняемый файл может лежать в `build/Debug` или `build/Release`.

## Управление

- `WASD` или стрелки — движение
- `Esc` — выход

## Этап 1

Сделано:

- базовая структура проекта;
- CMake со сборкой через несколько библиотек;
- окно SFML;
- игровой цикл;
- карта-сетка;
- игрок;
- простые столкновения со стенами.

Рекомендуемый коммит:

```bash
git add .
git commit -m "Add SFML game loop and player movement"
```

## Этап 2

Сделано:

- добавлен `EntityManager`;
- игровые объекты теперь хранятся в `std::vector<std::unique_ptr<Entity>>`;
- добавлен абстрактный класс `Enemy`;
- добавлен `ChaserEnemy`, который преследует игрока напрямую;
- использованы `std::move`, `std::make_unique`, range-based for, `final`, `override`, `const`;
- добавлен осмысленный шаблонный метод `EntityManager::create<T>()`.

Рекомендуемый коммит:

```bash
git add .
git commit -m "Add entity manager and chaser enemies"
```

## Этап 3

Сделано:

- добавлен класс `Pathfinder`;
- карта теперь умеет переводить координаты мира в клетки и обратно;
- враги используют A* и обходят стены;
- путь хранится как `std::vector<sf::Vector2i>`;
- для A* используются `std::priority_queue`, `std::unordered_map`, `std::vector`;
- добавлена отдельная библиотека `roboarena_pathfinding` в CMake;
- путь врага подсвечивается маленькими жёлтыми маркерами для отладки.

Почему враги раньше застревали:

- на втором этапе `ChaserEnemy` двигался по вектору `target - position`;
- если между врагом и игроком была стена, враг продолжал идти прямо в неё;
- теперь враг выбирает следующую клетку из найденного A* маршрута.

Рекомендуемый коммит:

```bash
git add .
git commit -m "Add A star pathfinding for enemies"
```
