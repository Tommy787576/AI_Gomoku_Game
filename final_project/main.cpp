#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <iostream>

#include "game.h"
#include "ai.h"

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

    Game game(15, 15);
    sf::Vector2f origin({ 3.4f, 3.4f });
    float offset = 41.6f;
    float radius = 19.0f;
    std::vector<std::vector<sf::CircleShape>> displayBoard(
        game.getWidthP(),
        std::vector<sf::CircleShape>(
            game.getHeightP(), sf::CircleShape(radius)
        )
    );
    for (int x = 0; x < game.getWidthP(); x++) {
        for (int y = 0; y < game.getHeightP(); y++) {
            float posX = (float)(x) * offset + origin.x;
            float posY = (float)(y) * offset + origin.y;
            displayBoard[x][y].setPosition(posX, posY);
            displayBoard[x][y].setFillColor(sf::Color::Transparent);
        }
    }

    std::map<Role, Player> role2Player{
        {Role::Black, Player::RandomAI},
        {Role::White, Player::Human}
    };
    AI blackAI, whiteAI;
    if (role2Player[Role::Black] != Player::Human)
        blackAI.setPlayer(role2Player[Role::Black]);
    if (role2Player[Role::White] != Player::Human)
        whiteAI.setPlayer(role2Player[Role::White]);
    bool isPut = false; // only one piece is added in a single loop

    while (window.isOpen()) {
        window.clear();
        window.draw(boardSprite);
        for (int x = 0; x < game.getWidthP(); x++)
            for (int y = 0; y < game.getHeightP(); y++)
                window.draw(displayBoard[x][y]);

        if (isPut) {
            isPut = false;
            game.switchRole();
        }
        if (game.getWinner() == Role::Black) {
            text.setString(L"黑棋勝利!");
            window.draw(text);
        }
        else if (game.getWinner() == Role::White) {
            text.setString(L"白棋勝利!");
            window.draw(text);
        }
        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (!game.isGameSet())
                    break;
                if (event.key.code == sf::Keyboard::Key::Enter) {
                    // restart the game
                    window.clear();
                    window.draw(boardSprite);
                    window.display();

                    game = Game();
                    for (int x = 0; x < game.getWidthP(); x++)
                        for (int y = 0; y < game.getHeightP(); y++)
                            displayBoard[x][y].setFillColor(
                                sf::Color::Transparent
                            );
                }
                break;
            case sf::Event::MouseButtonPressed:
                if (game.isGameSet())
                    break;
                if (isPut)
                    break;
                if (role2Player[game.getCurrRole()] != Player::Human)
                    break;
                for (int x = 0; x < game.getWidthP() && !isPut; x++) {
                    for (int y = 0; y < game.getHeightP() && !isPut; y++) {
                        if (displayBoard[x][y].getGlobalBounds().contains(
                            sf::Vector2f(sf::Mouse::getPosition(window)))) {
                            Role check = game.setPiece(x, y);
                            if (check == Role::Black) {
                                displayBoard[x][y].setFillColor(
                                    sf::Color::Black
                                );
                                isPut = true;
                            }
                            else if (check == Role::White) {
                                displayBoard[x][y].setFillColor(
                                    sf::Color::White
                                );
                                isPut = true;
                            }
                        }
                    }
                }
                break;
            }
        }
        if (!isPut && !game.isGameSet() &&
            role2Player[game.getCurrRole()] != Player::Human) {
            std::pair<int, int> nxtPos;
            if (game.getCurrRole() == Role::White)
                nxtPos = whiteAI.getNextPos(game);
            else
                nxtPos = blackAI.getNextPos(game);
            int x = nxtPos.first, y = nxtPos.second;
            Role check = game.setPiece(x, y);
            if (check == Role::Black) {
                displayBoard[x][y].setFillColor(
                    sf::Color::Black
                );
                isPut = true;
            }
            else if (check == Role::White) {
                displayBoard[x][y].setFillColor(
                    sf::Color::White
                );
                isPut = true;
            }
        }
    }

	return EXIT_SUCCESS;
}