#include "Arena.cpp"  // Arena sınıfı tanımı
#include "../include/config.h"  // Konfigürasyon sabitleri (ARENA_WIDTH, ARENA_HEIGHT)
#include <cstdlib>  // srand ve rand fonksiyonları için
#include <ctime>    // time fonksiyonu için

int main() {
    // Rastgele sayı üreticisinin başlatılması
    srand(time(0));

    // Arena nesnesi oluşturuluyor ve boyutlar parametre olarak veriliyor
    Arena arena(ARENA_WIDTH, ARENA_HEIGHT);

    // Arena başlatılıyor, robotlar ekleniyor vs.
    arena.initialize();

    // Oyun başlatılıyor
    arena.runGame();

    return 0;
}
