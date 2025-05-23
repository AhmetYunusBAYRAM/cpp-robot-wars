#ifndef ROBOT_CPP
#define ROBOT_CPP

#include "Movable.cpp"
#include "../include/config.h"

// Standart C++ kutuphaneleri
#include <iostream>   // Giris/cikis islemleri icin
#include <cstdlib>    // rand() fonksiyonu icin
#include <string>     // String islemleri icin
#include <ctime>
#include <cmath>

class Robot : public Movable {
protected:
    int id;
    static int nextId;

public:
    Robot(Point p, int type) : Movable(p, type, "R" + std::to_string(nextId++)), id(nextId-1) {}
};

// nextId değişkenini sadece bir kez tanımla
#ifdef ROBOT_CPP_IMPLEMENTATION
int Robot::nextId = 1;
#endif

class Player : public Movable {
public:
    Player(Point p, int index) : Movable(p, 1, "P" + std::to_string(index + 1)) {}
    
    Point move() override {
        std::cout << "Oyuncu " << nickName.substr(1) << " icin yon secin (0:Kuzey, 1:Dogu, 2:Guney, 3:Bati): ";
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
    Shooter(Point p, int index) : Movable(p, 2, "S" + std::to_string(index + 1)) {}
    
    Point move() override {
        return Point(location.getX() + 1, location.getY() - 1);
    }
};

class Freezer : public Movable {
public:
    Freezer(Point p, int index) : Movable(p, 3, "F" + std::to_string(index + 1)) {}
    
    Point move() override {
        int dx[] = {0, 1, 0, -1};
        int dy[] = {-1, 0, 1, 0};
        int r = rand() % 4;
        return Point(location.getX() + dx[r], location.getY() + dy[r]);
    }
};

class Jumper : public Movable {
public:
    Jumper(Point p, int index) : Movable(p, 4, "J" + std::to_string(index + 1)) {}
    
    Point move() override {
        return Point(rand() % ARENA_WIDTH, rand() % ARENA_HEIGHT);
    }
};

#endif
