#include "../include/Arena.h"
#include <cstdlib>
#include <ctime>

int main() {
    srand(time(0));
    Arena arena(20, 20); 
    arena.initialize();
    arena.runGame();
    return 0;
}