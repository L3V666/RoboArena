# RoboArena

RoboArena — 2D-игра на C++ и SFML. Игрок управляет роботом на арене, а враги преследуют его. В следующих этапах появятся поиск пути A*, снаряды, бонусы и интерфейс.

## Технологии

- C++17
- SFML
- CMake
- STL containers
- ООП: инкапсуляция, наследование, виртуальные методы

## Сборка

```bash
mkdir build
cd build
cmake ..
cmake --build .
./RoboArena
```

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
- добавлен `ChaserEnemy`, который преследует игрока;
- использованы `std::move`, `std::make_unique`, range-based for, `final`, `override`, `const`;
- добавлен осмысленный шаблонный метод `EntityManager::create<T>()`.