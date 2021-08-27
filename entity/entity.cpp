#include "entity.h"

Entity::Entity() : x(0), y(0), z(0) {}
Entity::Entity(int x, int y, int z) : x(x), y(y), z(z) {}

void Entity::moveXYZ(int x, int y, int z) {
    x = x;
    y = y;
    z = z;

    Logger::i("X:", x, " Y:", y, " Z:", z);
};