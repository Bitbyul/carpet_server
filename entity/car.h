#ifndef CARPET_SERVER_CAR_H
#define CARPET_SERVER_CAR_H
#include "../tools/cplogger.h"
#include "../handler/session.h"
#include "entity.h"

class Car : public Entity {

    double move_speed;
    double move_degree;

    bool in_move = false;


public:
    Car() = delete;

    Car(Session *session);

    ~Car();

    void move(double speed, double degree);
};


#endif //CARPET_SERVER_CAR_H
