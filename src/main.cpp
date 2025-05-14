#include "Arena.cpp"  // Arena sınıfı tanımı
#include <iostream>
#include <cstdlib>  // srand ve rand fonksiyonları için
#include <ctime>    // time fonksiyonu için

int main() {
    // Rastgele sayı üretecini başlat
    srand(time(nullptr));
    
    // Oyun alanı boyutlarını belirle
    const int WIDTH = 20;
    const int HEIGHT = 10;
    
    // Oyun alanını oluştur
    Arena arena(WIDTH, HEIGHT);
    
    // Oyunu başlat
    std::cout << "RobotWars'a Hoş Geldiniz!\n\n";
    arena.initialize();
    arena.runGame();
    
    return 0;
}
