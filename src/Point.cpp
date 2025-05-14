#ifndef POINT_CPP
#define POINT_CPP

#include <iostream>

class Point {
private:
    int x, y;
public:
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    int getX() const { return x; }
    int getY() const { return y; }
    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

#endif
