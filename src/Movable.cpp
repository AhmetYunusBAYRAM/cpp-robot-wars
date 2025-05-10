#include "../include/Movable.h"

Movable::Movable() : location(0,0), status(ALIVE), type(0), nickName(""), totalPoints(0) {}
Movable::Movable(Point p, int type, std::string nick)
    : location(p), status(ALIVE), type(type), nickName(nick), totalPoints(0) {}

void Movable::setLocation(Point p) { location = p; }
Point Movable::getLocation() const { return location; }
Status Movable::getStatus() const { return status; }
void Movable::setStatus(Status s) { status = s; }
std::string Movable::getNickName() const { return nickName; }
int Movable::getType() const { return type; }
int Movable::getTotalPoints() const { return totalPoints; }
void Movable::addPoints(int p) { totalPoints += p; }
void Movable::subPoints(int p) { totalPoints -= p; }