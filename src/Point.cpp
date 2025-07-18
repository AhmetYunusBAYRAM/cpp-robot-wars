#ifndef POINT_CPP
#define POINT_CPP

#include <iostream>

class Point {
private:
    int x, y;
public:

   Point(int xVal = 0, int yVal = 0) {
    x = xVal;
    y = yVal;
    }

    // Getter fonksiyonlarımız
    int getX() const { 
        return x; 
    }
    int getY() const { 
        return y; 
    }
    
    // Setter fonksiyonlarımız
    void setX(int x) { 
        this->x = x; 
    }
    void setY(int y) { 
        this->y = y; 
    }
    
    // Eşitlik operatörümüz
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

#endif
