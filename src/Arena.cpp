#ifndef ARENA_CPP
#define ARENA_CPP

#include "Robot.cpp"
#include "Movable.cpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <memory>

// ANSI renk kodları
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"

class Arena {
    private : 
    std::vector<std::shared_ptr<Movable>> robots;
    int width, height;

    // Verilen sembol karakterine göre renklendirilmiş bir string döner
    std::string getColoredSymbol(const std::string& symbol) {
        // Eğer sembol boşsa, renklendirme yapılmadan doğrudan geri döndürülür
        if (symbol.empty()) {
            return symbol;
        }

        // Robot tipine göre renklendirme
        switch(symbol[0]) {
            case 'P': return std::string(GREEN) + symbol + RESET;  // Oyuncu - Yeşil
            case 'S': return std::string(RED) + symbol + RESET;    // Nişancı - Kırmızı
            case 'F': return std::string(CYAN) + symbol + RESET;   // Dondurucu - Camgöbeği
            case 'J': return std::string(YELLOW) + symbol + RESET; // Zıplayan - Sarı
            default: return symbol;
        }
    }

    public:
    Arena(int arenaWidth, int arenaHeight) {
        width = arenaWidth;
        height = arenaHeight;
    }

    
    void initialize() {
    int np, ns, nf, nj;

    // Kullanıcıdan robot türü sayıları alınır
    std::cout << "Oyuncu sayısı ...........(1-9): ";
    std::cin >> np;

    std::cout << "Nişancı sayısı ..........(0-9): ";
    std::cin >> ns;

    std::cout << "Dondurucu sayısı ........(0-9): ";
    std::cin >> nf;

    std::cout << "Zıplayıcı sayısı ........(0-9): ";
    std::cin >> nj;

    // Oyuncuları oluştur ve listeye ekle
    for (int i = 0; i < np; i++) {
        robots.push_back(
            std::make_shared<Player>(
                Point(rand() % width, rand() % height), i
            )
        );
    }

    // Nişancıları oluştur ve listeye ekle
    for (int i = 0; i < ns; i++) {
        robots.push_back(
            std::make_shared<Shooter>(
                Point(rand() % width, rand() % height), i
            )
        );
    }

    // Dondurucuları oluştur ve listeye ekle
    for (int i = 0; i < nf; i++) {
        robots.push_back(
            std::make_shared<Freezer>(
                Point(rand() % width, rand() % height), i
            )
        );
    }

    // Zıplayıcıları oluştur ve listeye ekle
    for (int i = 0; i < nj; i++) {
        robots.push_back(
            std::make_shared<Jumper>(
                Point(rand() % width, rand() % height), i
            )
        );
    }
}

void runGame() {
    bool gameOver = false;
    while (!gameOver) {
        drawTerrain();
        showScoreTable();

        for (int i = 0; i < robots.size(); i++) {
            if (robots[i]->getStatus() == Movable::Status::ALIVE) {
                Point newLoc = robots[i]->move();
                // Sınırdan taşarsa diğer tarafa geçir
                int nx = (newLoc.getX() + width) % width;
                int ny = (newLoc.getY() + height) % height;
                robots[i]->setLocation(Point(nx, ny));
                int collidedWith = checkCollision(i);
                if (collidedWith > -1) {
                    robots[collidedWith]->setStatus(Movable::Status::DEAD);
                    robots[collidedWith]->subPoints(1);
                    robots[i]->addPoints(2);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        gameOver = checkPlayerStatus();
    }
    drawTerrain();
    showScoreTable();
    std::cout << "OYUN BİTTİ!\n";
}

void drawTerrain() {
    system("clear");

    // Üst kenar
    for (int x = 0; x < width + 2; x++) {
        if (x == 0) 
            std::cout << ".";
        else 
            std::cout << " .";
    }
    std::cout << "\n";

    // Oyun alanı
    for (int y = 0; y < height; y++) {
        std::cout << ".";
        for (int x = 0; x < width; x++) {
            bool yazildi = false;
            for (auto& r : robots) {
                if (r->getStatus() == Movable::Status::ALIVE && 
                    r->getLocation().getX() == x && 
                    r->getLocation().getY() == y) {
                    
                    std::string sym = r->getNickName();
                    std::cout << getColoredSymbol(sym);
                    yazildi = true;
                    break;
                }
            }
            if (!yazildi) {
                std::cout << "  "; 
            }
        }
        std::cout << " ." << "\n"; 
    }

    // Alt kenar
    for (int x = 0; x < width + 2; x++) {
        if (x == 0)
            std::cout << ".";
        else
            std::cout << " .";
    }
    std::cout << "\n";
}

void showScoreTable() {
    // Başlık yazdırılır
    std::cout << "Toplam Puan\n";

    // Tüm robotlar için skor ve durum bilgisi yazdırılır
    for (auto& r : robots) {
        // Robotun takma adı alınır
        std::string nickname = r->getNickName();

        // Robotun toplam puanı alınır
        int totalPoints = r->getTotalPoints();

        // Robotun durumu kontrol edilir
        std::string status = (r->getStatus() == Movable::Status::ALIVE) ? "CANLI" : "ÖLÜ";

        // Bilgiler ekrana yazdırılır
        std::cout << nickname << " = " << totalPoints << "   " << status << std::endl;
    }
}

int checkCollision(int idx) {
    // Tüm robotlar arasında çarpışma kontrolü yapılır
    for (int i = 0; i < robots.size(); i++) {
        
        // Kendi robotuyla çarpışma kontrolü yapılmaz
        if (i != idx) {
            
            // Robotun durumu CANLI ve konumu aynıysa, çarpışma var demektir
            if (robots[i]->getStatus() == Movable::Status::ALIVE && 
                robots[i]->getLocation() == robots[idx]->getLocation()) {
                
                // Çarpışma tespit edildiyse, çarpışan robotun indeksini döndür
                return i;
            }
        }
    }

    // Çarpışma tespit edilmediyse -1 döndürülür
    return -1;
}
bool checkPlayerStatus() {
    // Canlı robot sayısını tutacak değişken
    int alive = 0;

    // Tüm robotlar üzerinden geçilir
    for (auto& r : robots) {
        // Eğer robotun durumu ALIVE ise, canlı robot sayısı artırılır
        if (r->getStatus() == Movable::Status::ALIVE) {
            alive++;
        }
    }

    // Eğer canlı robot sayısı 1 veya daha az ise, oyunun sonlanma durumu gerçekleşmiştir
    return alive <= 1;
}

};

#endif