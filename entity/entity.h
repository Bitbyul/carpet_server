#ifndef CARPET_SERVER_ENTITY_H
#define CARPET_SERVER_ENTITY_H
#include "../tools/cplogger.h"

class Entity {
    int x;
    int y;
    int z;

public:
    Entity();
    Entity(int x, int y, int z);

    void moveXYZ(int x, int y, int z);
};


#endif //CARPET_SERVER_ENTITY_H
