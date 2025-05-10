#ifndef ARENA_H
#define ARENA_H

#include "Robot.h"
#include <vector>
#include <memory>

class Arena {
private:
    std::vector<std::shared_ptr<Movable>> robots;
    int width, height;
public:
    Arena(int w, int h);
    void initialize();
    void runGame();
    void drawTerrain();
    void showScoreTable();
    int checkCollision(int idx);
    bool checkPlayerStatus();
};

#endif