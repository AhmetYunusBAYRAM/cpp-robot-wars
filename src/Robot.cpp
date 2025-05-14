#ifndef ROBOT_CPP
#define ROBOT_CPP

#include <iostream>
#include <cstdlib>
#include <random>
#include <cmath>
#include <string>
#include "Movable.cpp"

class Player : public Movable {
public:
    Player(Point p, int index) : Movable(p, 1, "Player" + std::to_string(index)) {}
    
    Point move() override {
        std::cout << nickName << " için yön seçin (0:Kuzey, 1:Doğu, 2:Güney, 3:Batı): ";
        int dir; std::cin >> dir;
        Point newLoc = location;
        switch(dir) {
            case 0: newLoc.setY(location.getY() - 1); break;
            case 1: newLoc.setX(location.getX() + 1); break;
            case 2: newLoc.setY(location.getY() + 1); break;
            case 3: newLoc.setX(location.getX() - 1); break;
            default: break;
        }
        return newLoc;
    }
};

class Shooter : public Movable {
public:
    Shooter(Point p, int index) : Movable(p, 2, "Shooter" + std::to_string(index)) {}
    
    Point move() override {
        return Point(location.getX() + 1, location.getY() - 1);
    }
};

class Freezer : public Movable {
public:
    Freezer(Point p, int index) : Movable(p, 3, "Freezer" + std::to_string(index)) {}
    
    Point move() override {
        int dx[] = {0, 1, 0, -1};
        int dy[] = {-1, 0, 1, 0};
        int r = rand() % 4;
        return Point(location.getX() + dx[r], location.getY() + dy[r]);
    }
};

class Jumper : public Movable {
public:
    Jumper(Point p, int index) : Movable(p, 4, "Jumper" + std::to_string(index)) {}
    
    Point move() override {
        return Point(rand() % 20, rand() % 10);
    }
};

#endif
