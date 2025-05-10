#include "../include/Arena.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <memory>

// Renk kodları
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"

std::string getColoredSymbol(const std::string& symbol) {
    if (symbol[0] == 'P') return std::string(GREEN) + symbol + RESET;
    if (symbol[0] == 'S') return std::string(RED) + symbol + RESET;
    if (symbol[0] == 'F') return std::string(CYAN) + symbol + RESET;
    if (symbol[0] == 'J') return std::string(YELLOW) + symbol + RESET;
    return symbol;
}

Arena::Arena(int w, int h) : width(w), height(h) {}

void Arena::initialize() {
    int np, ns, nf, nj;
    std::cout << "Number of Players .....(1-9): "; std::cin >> np;
    std::cout << "Number of Shooters ...(0-9): "; std::cin >> ns;
    std::cout << "Number of Freezers ....(0-9): "; std::cin >> nf;
    std::cout << "Number of Jumpers .....(0-9): "; std::cin >> nj;
    for(int i=0;i<np;i++) robots.push_back(std::make_shared<Player>(Point(rand()%width,rand()%height),i));
    for(int i=0;i<ns;i++) robots.push_back(std::make_shared<Shooter>(Point(rand()%width,rand()%height),i));
    for(int i=0;i<nf;i++) robots.push_back(std::make_shared<Freezer>(Point(rand()%width,rand()%height),i));
    for(int i=0;i<nj;i++) robots.push_back(std::make_shared<Jumper>(Point(rand()%width,rand()%height),i));
}

void Arena::runGame() {
    bool gameOver = false;
    while (!gameOver) {
        drawTerrain();
        showScoreTable();

        for (int i = 0; i < robots.size(); i++) {
            if (robots[i]->getStatus() == ALIVE) {
                Point newLoc = robots[i]->move();
                // Sınırdan taşarsa diğer tarafa geçir
                int nx = (newLoc.getX() + width) % width;
                int ny = (newLoc.getY() + height) % height;
                robots[i]->setLocation(Point(nx, ny));
                int collidedWith = checkCollision(i);
                if (collidedWith > -1) {
                    robots[collidedWith]->setStatus(DEAD);
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
    std::cout << "GAME IS OVER!\n";
}

void Arena::drawTerrain() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    // Üst kenar (boşluksuz, sadece noktalar)
    for (int x = 0; x < width + 2; x++) {
        if (x == 0) 
            std::cout << ".";
        else 
            std::cout << " .";
    }

    std::cout << "\n";

    for (int y = 0; y < height; y++) {
        std::cout << ".";
        for (int x = 0; x < width; x++) {
            bool yazildi = false;
            for (auto& r : robots) {
                if (r->getStatus() == ALIVE &&
                    r->getLocation().getX() == x &&
                    r->getLocation().getY() == y) {
                    std::string sym = r->getNickName();
                    if (sym.length() < 2) sym += "0";
                    else if (sym.length() > 2) sym = sym.substr(0, 2);
                    std::cout << getColoredSymbol(sym);
                    yazildi = true;
                    break;
                }
            }
            if (!yazildi) std::cout << "  "; // Her hücre iki karakter
        }
        std::cout << " ." << "\n"; // Sağ kenar
    }

    // Alt kenar (boşluksuz, sadece noktalar)
    for (int x = 0; x < width + 2; x++) {
        if (x == 0) 
            std::cout << ".";
        else 
            std::cout << " .";
    }
    std::cout << "\n";
}



void Arena::showScoreTable() {
    std::cout << "Total Points\n";
    for (auto& r : robots) {
        std::cout << r->getNickName() << " = " << r->getTotalPoints() << "   "
                  << (r->getStatus() == ALIVE ? "ALIVE" : "DEAD") << std::endl;
    }
}

int Arena::checkCollision(int idx) {
    for (int i = 0; i < robots.size(); i++) {
        if (i != idx && robots[i]->getStatus() == ALIVE &&
            robots[i]->getLocation() == robots[idx]->getLocation())
            return i;
    }
    return -1;
}

bool Arena::checkPlayerStatus() {
    int alive = 0;
    for (auto& r : robots)
        if (r->getStatus() == ALIVE) alive++;
    return alive <= 1;
}