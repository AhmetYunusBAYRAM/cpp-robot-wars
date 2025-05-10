#ifndef ROBOT_H
#define ROBOT_H

#include "Movable.h"
#include <random>

class Player : public Movable {
public:
    Player(Point p, int index);
    Point move() override;
};

class Shooter : public Movable {
public:
    Shooter(Point p, int index);
    Point move() override;
};

class Freezer : public Movable {
public:
    Freezer(Point p, int index);
    Point move() override;
};

class Jumper : public Movable {
public:
    Jumper(Point p, int index);
    Point move() override;
};

#endif