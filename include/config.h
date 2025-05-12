#ifndef CONFIG_H
#define CONFIG_H

#include <string>

const int ARENA_WIDTH = 20;
const int ARENA_HEIGHT = 20;

enum RobotType {
    PLAYER,
    SHOOTER,
    FREEZER,
    JUMPER
};

enum RobotStatus {
    ALIVE,
    DEAD
};

enum Direction {
    NORTH = 0,
    NORTHEAST = 1,
    EAST = 2,
    SOUTHEAST = 3,
    SOUTH = 4,
    SOUTHWEST = 5,
    WEST = 6,
    NORTHWEST = 7
};

const std::string DIRECTION_NAMES[] = {
    "KUZEY",
    "KUZEYDOĞU",
    "DOĞU",
    "GÜNEYDOĞU",
    "GÜNEY",
    "GÜNEYBATI",
    "BATI",
    "KUZEYBATI"
};

#endif