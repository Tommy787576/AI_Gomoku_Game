#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "game.h"

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

    while (window.isOpen()) {
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
                bool isFind = false;
                for (int x = 0; x < game.getWidthP() && !isFind; x++) {
                    for (int y = 0; y < game.getHeightP() && !isFind; y++) {
                        if (displayBoard[x][y].getGlobalBounds().contains(
                            sf::Vector2f(sf::Mouse::getPosition(window)))) {
                            isFind = true;
                            Role check = game.setPiece(x, y);
                            if (check == Role::Black) {
                                displayBoard[x][y].setFillColor(
                                    sf::Color::Black
                                );
                            }
                            else if (check == Role::White) {
                                displayBoard[x][y].setFillColor(
                                    sf::Color::White
                                );
                            }
                        }
                    }
                }
                break;
            }
        }

        window.clear();
        window.draw(boardSprite);
        for (int x = 0; x < game.getWidthP(); x++)
            for (int y = 0; y < game.getHeightP(); y++)
                window.draw(displayBoard[x][y]);
        if (game.getWinner() == Role::Black) {
            text.setString(L"黑棋勝利!");
            window.draw(text);
        }
        else if (game.getWinner() == Role::White) {
            text.setString(L"白棋勝利!");
            window.draw(text);
        }
        window.display();
    }

	return EXIT_SUCCESS;
}