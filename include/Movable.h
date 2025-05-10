#ifndef MOVABLE_H
#define MOVABLE_H

#include "Point.h"
#include <string>

enum Status { ALIVE, DEAD };

class Movable {
protected:
    Point location;
    Status status;
    int type;
    std::string nickName;
    int totalPoints;
public:
    Movable();
    Movable(Point p, int type, std::string nick);
    void setLocation(Point p);
    Point getLocation() const;
    Status getStatus() const;
    void setStatus(Status s);
    std::string getNickName() const;
    int getType() const;
    int getTotalPoints() const;
    void addPoints(int p);
    void subPoints(int p);
    virtual Point move() = 0;
};

#endif