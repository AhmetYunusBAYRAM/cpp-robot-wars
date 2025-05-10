#include "../include/Robot.h"
#include <iostream>
#include <cstdlib>

// Player
Player::Player(Point p, int index) : Movable(p, 0, "P" + std::to_string(index)) {}
Point Player::move() {
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

// Shooter
Shooter::Shooter(Point p, int index) : Movable(p, 1, "S" + std::to_string(index)) {}
Point Shooter::move() {
    // Diyagonal hareket (örnek: sağ üst)
    return Point(location.getX() + 1, location.getY() - 1);
}

// Freezer
Freezer::Freezer(Point p, int index) : Movable(p, 2, "F" + std::to_string(index)) {}
Point Freezer::move() {
    int dx[] = {0,1,0,-1}, dy[] = {-1,0,1,0};
    int r = rand() % 4;
    return Point(location.getX() + dx[r], location.getY() + dy[r]);
}

// Jumper
Jumper::Jumper(Point p, int index) : Movable(p, 3, "J" + std::to_string(index)) {}
Point Jumper::move() {
    return Point(rand()%20, rand()%10);
}