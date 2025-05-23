#include "Arena.cpp"  // Arena sınıfı tanımı
#include "../include/config.h"
#include <iostream>
#include <cstdlib>  // srand ve rand fonksiyonları için
#include <ctime>    // time fonksiyonu için
#include <windows.h>

int main() {
    // Windows konsol ayarları
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;
    GetConsoleScreenBufferInfo(hOut, &scrBufferInfo);
    
    // Konsol boyutunu ayarla
    SMALL_RECT winSize = {0, 0, 100, 30};
    SetConsoleWindowInfo(hOut, TRUE, &winSize);
    
    // Rastgele sayı üretecini başlat
    srand(time(nullptr));
    
    // Oyun alanını oluştur
    Arena arena(ARENA_WIDTH, ARENA_HEIGHT);
    
    // Oyunu başlat
    arena.initialize();
    
    // Oyunu çalıştır
    arena.runGame();
    
    return 0;
}
