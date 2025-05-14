#ifndef MOVABLE_CPP
#define MOVABLE_CPP

#include <iostream>
#include <string>
#include "Point.cpp"

class Movable {
public:
    enum Status { ALIVE, DEAD };
    
    Point location;
    Status status;
    int type;
    std::string nickName;
    int totalPoints;
    
    Movable() : status(ALIVE), type(0), totalPoints(0) {}
    Movable(Point p, int type, std::string nick) 
        : location(p), status(ALIVE), type(type), nickName(nick), totalPoints(0) {}
    
    void setLocation(Point p) { location = p; }
    Point getLocation() const { return location; }
    Status getStatus() const { return status; }
    void setStatus(Status s) { status = s; }
    std::string getNickName() const { return nickName; }
    int getType() const { return type; }
    int getTotalPoints() const { return totalPoints; }
    void addPoints(int p) { totalPoints += p; }
    void subPoints(int p) { totalPoints -= p; }
    virtual Point move() = 0;
};

#endif