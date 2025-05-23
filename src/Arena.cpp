#ifndef ARENA_CPP
#define ARENA_CPP

#include "Robot.cpp"
#include "Movable.cpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <memory>
#include <iomanip>
#include <algorithm> // std::max için

// Renk kodları
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define ORANGE  "\033[38;5;208m"
#define BG_BLACK "\033[40m"

// Basit karakterler
#define TOP_LEFT     "+"
#define TOP_RIGHT    "+"
#define BOTTOM_LEFT  "+"
#define BOTTOM_RIGHT "+"
#define HORIZONTAL   "-"
#define VERTICAL     "|"
#define EMPTY_SPACE  "."

class Arena {
    private : 
    std::vector<std::shared_ptr<Movable>> robots;
    int width, height;

    // Ekran genişliğini al
    int getTerminalWidth() {
        return 80; // Sabit genişlik
    }

    // Belirtilen sayıda boşluk yazdır
    void printSpaces(int count) {
        for(int i = 0; i < count; i++) {
            std::cout << " ";
        }
    }

    // Robot bilgilerini yazdır
    void printRobotInfo(std::shared_ptr<Movable> robot) {
        if (robot) {
            std::string status = (robot->getStatus() == Movable::Status::ALIVE) ? 
                (GREEN + std::string(" AKTIF") + RESET) : 
                (RED + std::string(" PASIF") + RESET);
            std::string info = robot->getNickName() + " : " + std::to_string(robot->getTotalPoints());
            
            // Nickname uzunluğuna göre sabit boşluk ekle
            int nicknameLength = robot->getNickName().length();
            int requiredPadding = 3; // Sabit boşluk
            if (nicknameLength == 1) requiredPadding = 4; // P1, S1 gibi tek karakterli nickname'ler için 4 boşluk
            
            for(int i = 0; i < requiredPadding; ++i) {
                std::cout << " ";
            }

            // Bilgiyi ve durumu belirli bir genişlikte yazdır
            std::cout << std::left << std::setw(17) << (info + status);
        } else {
            // Boş satırlar için aynı genişlikte boşluk yazdır
            for(int i = 0; i < 4; ++i) {
                 std::cout << " ";
            }
            std::cout << std::setw(17) << " ";
        }
    }

    // Verilen sembol karakterine göre renklendirilmiş bir string döner
    std::string getColoredSymbol(const std::string& symbol) {
        if (symbol.empty()) {
            return symbol;
        }
        
        // İlk karaktere göre arka plan rengi belirle
        char firstChar = symbol[0];
        std::string bgColor;
        
        if (firstChar == 'P') {
            bgColor = std::string("\033[42m");  // Yeşil arka plan
        } else if (firstChar == 'S') {
            bgColor = std::string("\033[41m");  // Kırmızı arka plan
        } else if (firstChar == 'F') {
            bgColor = std::string("\033[46m");  // Cyan arka plan
        } else if (firstChar == 'J') {
            bgColor = std::string("\033[43m");  // Sarı arka plan
        }
        
        return bgColor + symbol + std::string(RESET);
    }

    public:
    Arena(int arenaWidth, int arenaHeight) {
        width = arenaWidth;  
        height = arenaHeight;
    }

    
    void initialize() {
        system("cls");  // Windows için clear komutu
        printHeader();
        int np, ns, nf, nj;

        // Kullanıcıdan robot türü sayıları alınır
        std::cout << "OYUNCU SAYISI ...........(1-9): ";
        std::cin >> np;
        if (np < 1) np = 1;
        if (np > 9) np = 9;

        std::cout << "NISANCI SAYISI ..........(0-9): ";
        std::cin >> ns;
        if (ns < 0) ns = 0;
        if (ns > 9) ns = 9;

        std::cout << "DONDURUCU SAYISI ........(0-9): ";
        std::cin >> nf;
        if (nf < 0) nf = 0;
        if (nf > 9) nf = 9;

        std::cout << "ZIPLAYICI SAYISI ........(0-9): ";
        std::cin >> nj;
        if (nj < 0) nj = 0;
        if (nj > 9) nj = 9;

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
    int turn = 1;
    bool gameOver = false;
    bool firstTurn = true;
    while (!gameOver) {
        if (firstTurn) {
            showTurnAnimation(turn);
            firstTurn = false;
        }
        drawTerrain();
        std::cout << "\n\n";  // İki satır boşluk ekle
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
        turn++;
    }
    drawTerrain();
    std::cout << "\n\n";  // İki satır boşluk ekle
    showScoreTable();
    std::cout << "OYUN BİTTİ!\n";
}

void printHeader() {
    int screenWidth = getTerminalWidth();
    int mapWidth = (width * 2);
    std::string title = "ROBOT SAVASLARI";
    int boxPadding = (screenWidth - mapWidth) / 2;
    int titlePadding = (mapWidth - 2 - title.length()) / 2;
    int rightPadding = (mapWidth - 2 - title.length()) - titlePadding;

    // Kutunun üst çizgisi
    std::string ust = std::string(boxPadding, ' ') + ORANGE + BG_BLACK + "+" + std::string(mapWidth - 2, '-') + "+" + RESET;
    for (char c : ust) { std::cout << c << std::flush; std::this_thread::sleep_for(std::chrono::milliseconds(2)); }
    std::cout << "\n";

    // Başlık satırı
    std::string satir = std::string(boxPadding, ' ') + ORANGE + BG_BLACK + "|" + RESET + std::string(titlePadding, ' ') + title + std::string(rightPadding, ' ') + ORANGE + BG_BLACK + "|" + RESET;
    for (char c : satir) { std::cout << c << std::flush; std::this_thread::sleep_for(std::chrono::milliseconds(2)); }
    std::cout << "\n";

    // Kutunun alt çizgisi
    std::string alt = std::string(boxPadding, ' ') + ORANGE + BG_BLACK + "+" + std::string(mapWidth - 2, '-') + "+" + RESET;
    for (char c : alt) { std::cout << c << std::flush; std::this_thread::sleep_for(std::chrono::milliseconds(2)); }
    std::cout << "\n\n";
}

void drawTerrain() {
    system("cls");  // Windows için clear komutu
    
    int screenWidth = getTerminalWidth();
    int mapWidth = (width * 2);
    int padding = (screenWidth - mapWidth) / 2;

    // Üst kenar
    printSpaces(padding);
    std::cout << ORANGE << TOP_LEFT;
    for (int x = 0; x < width; x++) {
        std::cout << HORIZONTAL << HORIZONTAL;
    }
    std::cout << TOP_RIGHT << RESET << "\n";

    // Oyun alanı
    for (int y = 0; y < height; y++) {
        printSpaces(padding);
        std::cout << ORANGE << VERTICAL << RESET;
        for (int x = 0; x < width; x++) {
            bool yazildi = false;
            for (auto& r : robots) {
                if (r->getStatus() == Movable::Status::ALIVE && 
                    r->getLocation().getX() == x && 
                    r->getLocation().getY() == y) {
                    std::string sym = r->getNickName();
                    // Sembolü ortala
                    int remainingSpace = 2 - sym.length();
                    int leftSpace = remainingSpace / 2;
                    int rightSpace = remainingSpace - leftSpace;
                    
                    // Sol boşluk
                    for(int i = 0; i < leftSpace; i++) {
                        std::cout << " ";
                    }
                    // Sembol
                    std::cout << getColoredSymbol(sym);
                    // Sağ boşluk
                    for(int i = 0; i < rightSpace; i++) {
                        std::cout << " ";
                    }
                    yazildi = true;
                    break;
                }
            }
            if (!yazildi) {
                std::cout << EMPTY_SPACE << " "; 
            }
        }
        std::cout << ORANGE << VERTICAL << RESET << "\n";
    }

    // Alt kenar
    printSpaces(padding);
    std::cout << ORANGE << BOTTOM_LEFT;
    for (int x = 0; x < width; x++) {
        std::cout << HORIZONTAL << HORIZONTAL;
    }
    std::cout << BOTTOM_RIGHT << RESET << "\n";
}

void showScoreTable() {
    int screenWidth = getTerminalWidth();
    int tableWidth = 83;
    int padding = (screenWidth - tableWidth) / 2;

    std::vector<std::shared_ptr<Movable>> players, shooters, freezers, jumpers;
    for (auto& r : robots) {
        if (dynamic_cast<Player*>(r.get())) {
            players.push_back(r);
        } else if (dynamic_cast<Shooter*>(r.get())) {
            shooters.push_back(r);
        } else if (dynamic_cast<Freezer*>(r.get())) {
            freezers.push_back(r);
        } else if (dynamic_cast<Jumper*>(r.get())) {
            jumpers.push_back(r);
        }
    }

    // Her kategori için 9 sütun oluştur
    const int MAX_COLUMNS = 9;

    printSpaces(padding);
    std::cout << std::left 
              << std::setw(20) << (GREEN + std::string("      OYUNCULAR") + RESET)
              << std::setw(20) << (RED + std::string("          NISANCILAR") + RESET)
              << std::setw(20) << (CYAN + std::string("        DONDURUCULAR") + RESET)
              << std::setw(20) << (YELLOW + std::string("       ZIPLAYANLAR") + RESET) << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    printSpaces(padding);
    std::cout << ORANGE << std::string(80, '-') << RESET << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(30));

    // Her kategori için 9 satır oluştur
    for (int i = 0; i < MAX_COLUMNS; i++) {
        printSpaces(padding + 2);
        printRobotInfo(i < players.size() ? players[i] : nullptr);
        std::cout << "   ";
        printRobotInfo(i < shooters.size() ? shooters[i] : nullptr);
        std::cout << "   ";
        printRobotInfo(i < freezers.size() ? freezers[i] : nullptr);
        std::cout << "   ";
        printRobotInfo(i < jumpers.size() ? jumpers[i] : nullptr);
        std::cout << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
    std::cout << "\n";
}

void showTurnAnimation(int turn) {
    std::string mesaj = "YENI TUR BASLIYOR!";
    int screenWidth = getTerminalWidth();
    int padding = (screenWidth - mesaj.length()) / 2;
    system("cls");  // Windows için clear komutu
    for (int i = 0; i < padding; ++i) std::cout << " ";
    std::cout << YELLOW;
    for (char c : mesaj) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
    std::cout << RESET << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(700));
    system("cls");  // Windows için clear komutu
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