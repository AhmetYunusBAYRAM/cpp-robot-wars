#include "Arena.cpp"  // Arena sınıfı tanımı
#include "../include/config.h"
#include <iostream>
#include <cstdlib>  // srand ve rand fonksiyonları için
#include <ctime>    // time fonksiyonu için
#include <windows.h>

int main() {
    // Windows'ta ANSI renk kodlarını etkinleştir
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

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
