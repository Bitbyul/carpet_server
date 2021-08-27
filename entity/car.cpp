#include "car.h"

Car::Car(Session *session): Entity(), move_speed(0), move_degree(0){
    Logger::i("Car is created.");
}
Car::~Car(){
    Logger::i("Car is destroyed.");
}

void Car::move(double speed, double degree) {
    move_speed = speed;
    move_degree = degree;

    Logger::i("Speed:", move_speed, " Degree:", move_degree);
}