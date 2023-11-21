#pragma once
#include <vector>

enum class Role {
    Black,
    White,
    Empty,
    Occupied
};

class Game {
private:
    std::vector<std::vector<Role>> board;
    int boardWidthP;
    int boardHeightP;
    Role currRole;
    Role winner;
public:
    Game(int _boardWidthP = 15, int _boardHeightP = 15);
    bool isInBoard(int x, int y);
    bool isGameSet();
    int getWidthP();
    int getHeightP();
    Role getWinner();
    Role getCurrRole();
    std::vector<std::vector<Role>>& getCurrBoard();
    void switchRole();
    Role setPiece(int x, int y);
    bool isWin();
};