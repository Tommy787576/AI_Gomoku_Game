#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

enum class Role {
    Black,
    White,
    Empty
};

struct Piece {
    Piece(float posX, float posY, float radius) :
        shape(sf::CircleShape(radius)), role(Role::Empty) {
        shape.setPosition(posX, posY);
        shape.setFillColor(sf::Color::Transparent);
    }
    sf::CircleShape shape;
    Role role;
};

int boardWidthP = 15, boardHeightP = 15;
inline bool isInBoard(int x, int y) {
    return x >= 0 && x < boardWidthP && y >= 0 && y < boardHeightP;
}
bool isWin(std::vector<std::vector<Piece>>& board, Role currRole) {
    std::vector<std::pair<int, int>> diffs{
        {1, -1},    // check up-right
        {1, 0},     // check right
        {1, 1},     // check bottom-right
        {0, 1}      // check bottom
    };
    for (int x = 0; x < boardWidthP; x++) {
        for (int y = 0; y < boardHeightP; y++) {
            if (board[x][y].role == currRole) {
                for (int i = 0; i < 4; i++) {
                    for (int j = 1; j <= 5; j++) {
                        if (j == 5)
                            return true;
                        int nxtX = x + diffs[i].first * j;
                        int nxtY = y + diffs[i].second * j;
                        if (!isInBoard(nxtX, nxtY) ||
                            board[nxtX][nxtY].role != currRole)
                            break;
                    }
                }
            }
        }
    }
    return false;
}

int main(void) {
    sf::Font font;
    if (!font.loadFromFile("msjh.ttc")) {
        std::cout << "Fail to load msjh.ttc" << std::endl;
        return EXIT_FAILURE;
    }
    sf::Text text("", font);
    text.setFillColor(sf::Color::Black);

	sf::Texture boardTexture;
    if (!boardTexture.loadFromFile("board.png")) {
        std::cout << "Fail to load board.png" << std::endl;
        return EXIT_FAILURE;
    }
	sf::Sprite boardSprite(boardTexture);
    sf::Vector2f sz = boardSprite.getGlobalBounds().getSize();
    unsigned int boardWidth = (unsigned int)(sz.x);
    unsigned int boardHeight = (unsigned int)(sz.y);
    sf::RenderWindow window(
        sf::VideoMode(boardWidth, boardHeight),
        L"五子棋"
    );

    sf::Vector2f origin({ 3.4f, 3.4f });
    float offset = 41.6f;
    float radius = 19.0f;
    std::vector<std::vector<Piece>> board(boardHeightP, std::vector<Piece>());
    for (int x = 0; x < boardWidthP; x++) {
        for (int y = 0; y < boardHeightP; y++) {
            float posX = (float)(x) * offset + origin.x;
            float posY = (float)(y) * offset + origin.y;
            board[x].push_back(Piece(posX, posY, radius));
        }
    }

    Role currRole = Role::Black;
    Role winner = Role::Empty;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (winner == Role::Empty)
                    break;
                if (event.key.code == sf::Keyboard::Key::Enter) {
                    // restart the game
                    for (int x = 0; x < boardWidthP; x++) {
                        for (int y = 0; y < boardHeightP; y++) {
                            board[x][y].shape.setFillColor(
                                sf::Color::Transparent
                            );
                            board[x][y].role = Role::Empty;
                        }
                    }
                    currRole = Role::Black;
                    winner = Role::Empty;
                }
                break;
            case sf::Event::MouseButtonPressed:
                if (winner != Role::Empty)
                    break;
                bool isFind = false;
                for (int x = 0; x < boardWidthP && !isFind; x++) {
                    for (int y = 0; y < boardHeightP && !isFind; y++) {
                        if (board[x][y].shape.getGlobalBounds().contains(
                            sf::Vector2f(sf::Mouse::getPosition(window)))) {
                            isFind = true;
                            if (board[x][y].role == Role::Empty) {
                                std::cout << x << ' ' << y << '\n';
                                if (currRole == Role::Black) {
                                    board[x][y].shape.setFillColor(
                                        sf::Color::Black
                                    );
                                    board[x][y].role = Role::Black;
                                }
                                else {
                                    board[x][y].shape.setFillColor(
                                        sf::Color::White
                                    );
                                    board[x][y].role = Role::White;
                                }
                                if (isWin(board, currRole))
                                    winner = currRole;
                                currRole = currRole == Role::Black ?
                                    Role::White : Role::Black;
                            }
                        }
                    }
                }
                break;
            }
        }

        window.clear();
        window.draw(boardSprite);
        for (int x = 0; x < boardWidthP; x++)
            for (int y = 0; y < boardHeightP; y++)
                window.draw(board[x][y].shape);
        if (winner == Role::Black) {
            text.setString(L"黑棋勝利!");
            window.draw(text);
        }
        else if (winner == Role::White) {
            text.setString(L"白棋勝利!");
            window.draw(text);
        }
        window.display();
    }

	return EXIT_SUCCESS;
}