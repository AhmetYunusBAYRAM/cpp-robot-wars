#ifndef ARENA_CPP
#define ARENA_CPP

#include "Robot.cpp"
#include "Movable.cpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <memory>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iomanip>

// ANSI renk kodları
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define BG_BLACK "\033[40m"
#define BG_WHITE "\033[47m"
#define ORANGE  "\033[38;5;208m"

// Özel karakterler
#define TOP_LEFT     "╔"
#define TOP_RIGHT    "╗"
#define BOTTOM_LEFT  "╚"
#define BOTTOM_RIGHT "╝"
#define HORIZONTAL   "═"
#define VERTICAL     "║"
#define EMPTY_SPACE  "·"

class Arena {
    private : 
    std::vector<std::shared_ptr<Movable>> robots;
    int width, height;

    // Ekran genişliğini al
    int getTerminalWidth() {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        return w.ws_col;
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
            std::string status = (robot->getStatus() == Movable::Status::ALIVE) ? " AKTİF" : "PASİF";
            std::string bgColor = (robot->getStatus() == Movable::Status::ALIVE) ? "\033[42m" : "\033[41m";
            std::cout << "   ";  // 3 boşluk
            std::string info = robot->getNickName() + " : " + std::to_string(robot->getTotalPoints());
            if (robot->getTotalPoints() >= -9 && robot->getTotalPoints() < 10) info += " ";
            std::cout << bgColor << std::left << std::setw(17) << (info + status) << RESET;
        } else {
            // Boş kutuda sadece boşluk bırak, arka plan rengi verme
            std::cout << "   " << std::setw(16) << " ";
        }
    }

    // Verilen sembol karakterine göre renklendirilmiş bir string döner
    std::string getColoredSymbol(const std::string& symbol) {
        if (symbol.empty()) {
            return symbol;
        }

        // Sembolün numarasını al (örn: "P1" -> "1")
        std::string number = symbol.substr(1);

        // Arka plan renk kodları
        const std::string BG_GREEN = "\033[30;42m";   // Siyah yazı, yeşil arka plan
        const std::string BG_RED = "\033[30;41m";     // Siyah yazı, kırmızı arka plan
        const std::string BG_CYAN = "\033[30;46m";    // Siyah yazı, camgöbeği arka plan
        const std::string BG_YELLOW = "\033[30;43m";  // Siyah yazı, sarı arka plan

        switch(symbol[0]) {
            case 'P': return BG_GREEN + "P" + number + RESET;   // Oyuncu
            case 'S': return BG_RED + "S" + number + RESET;     // Nişancı
            case 'F': return BG_CYAN + "F" + number + RESET;    // Dondurucu
            case 'J': return BG_YELLOW + "J" + number + RESET;  // Zıplayan
            default: return symbol;
        }
    }

    public:
    Arena(int arenaWidth, int arenaHeight) {
        width = (arenaWidth < 10) ? 10 : arenaWidth;
        height = (arenaHeight < 10) ? 10 : arenaHeight;
    }

    
    void initialize() {
        system("cls");
        printHeader();
        int np, ns, nf, nj;

        // Kullanıcıdan robot türü sayıları alınır
        std::cout << "Oyuncu sayısı ...........(1-9): ";
        std::cin >> np;
        if (np < 1) np = 1;
        if (np > 9) np = 9;

        std::cout << "Nişancı sayısı ..........(0-9): ";
        std::cin >> ns;
        if (ns < 0) ns = 0;
        if (ns > 9) ns = 9;

        std::cout << "Dondurucu sayısı ........(0-9): ";
        std::cin >> nf;
        if (nf < 0) nf = 0;
        if (nf > 9) nf = 9;

        std::cout << "Zıplayıcı sayısı ........(0-9): ";
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
    int mapWidth = (width * 2) + 2;
    std::string title = "ROBOT SAVAŞLARI";
    int boxPadding = (screenWidth - mapWidth) / 2;
    int titlePadding = (mapWidth - 2 - title.length()) / 2;
    int rightPadding = (mapWidth - 2 - title.length()) - titlePadding;

    // Kutunun üst çizgisi
    std::string ust = std::string(boxPadding, ' ') + ORANGE + BG_BLACK + "+" + std::string(mapWidth - 2, '-') + "+" + RESET;
    for (char c : ust) { std::cout << c << std::flush; std::this_thread::sleep_for(std::chrono::milliseconds(2)); }
    std::cout << "\n";

    // Başlık satırı
    std::string satir = std::string(boxPadding, ' ') + ORANGE + BG_BLACK + "|" + RESET + std::string(titlePadding, ' ') + title + std::string(rightPadding, ' ') + ORANGE + BG_BLACK + " |" + RESET;
    for (char c : satir) { std::cout << c << std::flush; std::this_thread::sleep_for(std::chrono::milliseconds(2)); }
    std::cout << "\n";

    // Kutunun alt çizgisi
    std::string alt = std::string(boxPadding, ' ') + ORANGE + BG_BLACK + "+" + std::string(mapWidth - 2, '-') + "+" + RESET;
    for (char c : alt) { std::cout << c << std::flush; std::this_thread::sleep_for(std::chrono::milliseconds(2)); }
    std::cout << "\n\n";
}

void drawTerrain() {
    system("cls");
    printHeader();
    
    int screenWidth = getTerminalWidth();
    int mapWidth = (width * 2) + 2;
    int padding = (screenWidth - mapWidth) / 2;

    // Üst kenar
    printSpaces(padding);
    std::cout << ORANGE << BG_BLACK << TOP_LEFT;
    for (int x = 0; x < width; x++) {
        std::cout << HORIZONTAL << HORIZONTAL;
    }
    std::cout << TOP_RIGHT << RESET << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    // Oyun alanı
    for (int y = 0; y < height; y++) {
        printSpaces(padding);
        std::cout << ORANGE << BG_BLACK << VERTICAL << RESET;
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
                std::cout << EMPTY_SPACE << " "; 
            }
        }
        std::cout << ORANGE << BG_BLACK << VERTICAL << RESET << "\n"; 
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    // Alt kenar
    printSpaces(padding);
    std::cout << ORANGE << BG_BLACK << BOTTOM_LEFT;
    for (int x = 0; x < width; x++) {
        std::cout << HORIZONTAL << HORIZONTAL;
    }
    std::cout << BOTTOM_RIGHT << RESET << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

void showScoreTable() {
    int screenWidth = getTerminalWidth();
    int tableWidth = 83;
    int padding = (screenWidth - tableWidth) / 2 + 2;

    std::vector<std::shared_ptr<Movable>> players, shooters, freezers, jumpers;
    for (auto& r : robots) {
        switch(r->getNickName()[0]) {
            case 'P': players.push_back(r); break;
            case 'S': shooters.push_back(r); break;
            case 'F': freezers.push_back(r); break;
            case 'J': jumpers.push_back(r); break;
        }
    }

    printSpaces(padding);
    std::cout << std::left 
              << std::setw(20) << (GREEN + std::string("      Oyuncular") + RESET)
              << std::setw(20) << (RED + std::string("          Nişancılar") + RESET)
              << std::setw(20) << (CYAN + std::string("        Dondurucular") + RESET)
              << std::setw(20) << (YELLOW + std::string("       Zıplayanlar") + RESET) << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    printSpaces(padding);
    std::cout << ORANGE << std::string(80, '-') << RESET << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(30));

    int maxSize = std::max({players.size(), shooters.size(), freezers.size(), jumpers.size()});

    for (int i = 0; i < maxSize; i++) {
        printSpaces(padding);
        printRobotInfo(i < players.size() ? players[i] : nullptr);
        printRobotInfo(i < shooters.size() ? shooters[i] : nullptr);
        printRobotInfo(i < freezers.size() ? freezers[i] : nullptr);
        printRobotInfo(i < jumpers.size() ? jumpers[i] : nullptr);
        std::cout << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
    std::cout << "\n";
}

void showTurnAnimation(int turn) {
    std::string mesaj = "Yeni Tur Başlıyor!";
    int screenWidth = getTerminalWidth();
    int padding = (screenWidth - mesaj.length()) / 2;
    system("cls");
    for (int i = 0; i < padding; ++i) std::cout << " ";
    std::cout << YELLOW;
    for (char c : mesaj) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
    std::cout << RESET << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(700));
    system("cls");
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