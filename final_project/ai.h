#pragma once
#include <vector>
#include <chrono>
#include <thread>
#include <ctime> 
#include "game.h"

enum class Player {
    Human,
    RandomAI,
    NormalAI,
    HardAI
};

class AI {
private:
    Player player;
public:
    void setPlayer(Player p) {
        player = p;
    }
    std::pair<int, int> getNextPos(Game& game) {
        std::vector<std::pair<int, int>> emptyPlace;
        for (int x = 0; x < game.getWidthP(); x++) {
            for (int y = 0; y < game.getHeightP(); y++) {
                if (game.getCurrBoard()[x][y] == Role::Empty)
                    emptyPlace.push_back({ x, y });
            }
        }
        std::this_thread::sleep_for(std::chrono::nanoseconds(100000000));
        srand((unsigned int)(time(NULL)));
        return emptyPlace[rand() % emptyPlace.size()];
    }
};

