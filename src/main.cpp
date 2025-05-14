#include "Arena.cpp"  // Arena sınıfı tanımı
#include "../include/config.h"
#include <iostream>
#include <cstdlib>  // srand ve rand fonksiyonları için
#include <ctime>    // time fonksiyonu için

int main() {
    // Rastgele sayı üretecini başlat
    srand(time(nullptr));
    
    // Oyun alanını oluştur
    Arena arena(ARENA_WIDTH, ARENA_HEIGHT);
    
    arena.initialize();
    arena.runGame();
    
    return 0;
}
