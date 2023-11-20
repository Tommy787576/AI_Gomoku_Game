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
    Game(int _boardWidthP = 15, int _boardHeightP = 15):
        boardWidthP(_boardWidthP), boardHeightP(_boardHeightP),
        currRole(Role::Black), winner(Role::Empty) {
        board.resize(boardHeightP, 
            std::vector<Role>(boardWidthP, Role::Empty));
    }
    inline bool isInBoard(int x, int y) {
        return x >= 0 && x < boardWidthP && y >= 0 && y < boardHeightP;
    }
    inline bool isGameSet() {
        return winner != Role::Empty;
    }
    inline int getWidthP() {
        return boardWidthP;
    }
    inline int getHeightP() {
        return boardHeightP;
    }
    inline Role getWinner() {
        return winner;
    }
    Role setPiece(int x, int y) {
        if (board[x][y] == Role::Empty) {
            if (currRole == Role::Black)
                board[x][y] = Role::Black;
            else
                board[x][y] = Role::White;
            if (isWin())
                winner = currRole;
            Role prevRole = currRole;
            currRole = currRole == Role::Black ?
                Role::White : Role::Black;
            return prevRole;
        }
        return Role::Occupied;
    }
    bool isWin() {
        std::vector<std::pair<int, int>> diffs{
            {1, -1},    // check up-right
            {1, 0},     // check right
            {1, 1},     // check bottom-right
            {0, 1}      // check bottom
        };
        for (int x = 0; x < boardWidthP; x++) {
            for (int y = 0; y < boardHeightP; y++) {
                if (board[x][y] == currRole) {
                    for (int i = 0; i < 4; i++) {
                        for (int j = 1; j <= 5; j++) {
                            if (j == 5)
                                return true;
                            int nxtX = x + diffs[i].first * j;
                            int nxtY = y + diffs[i].second * j;
                            if (!isInBoard(nxtX, nxtY) ||
                                board[nxtX][nxtY] != currRole)
                                break;
                        }
                    }
                }
            }
        }
        return false;
    }
};