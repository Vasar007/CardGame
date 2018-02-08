#include <SFML/Graphics.hpp>
#include <string>
#include <ctime>
#include <vector>
#include <thread>
#include "menu.h"

#define COUNT 13
#define ALL 130

/// Game function.
bool startGame(sf::RenderWindow &window, int &numberLevel, bool &showMainMenu)
{
	/// Code block which initialized program.
	if (showMainMenu)
		menu(window);			// Launch main menu.

	srand(time(NULL));

	char part1 = (char)92, part2 = '1', part3 = '-';
	std::string part = "Textures";
	part += part1;
	part += part2;
	part += part3;
	std::string ras = ".jpg";
	sf::Image cardImg[COUNT];
	for (int i = 0; i < COUNT; i++) {
		std::string pathCard = part + std::to_string(i + 1) + ras;
		cardImg[i].loadFromFile(pathCard);
	}
	sf::Texture cardText[COUNT];
	for (int i = 0; i < COUNT; i++)
		cardText[i].loadFromImage(cardImg[i]);
	sf::Sprite cardSpr[COUNT];
	for (int i = 0; i < COUNT; i++)
		cardSpr[i].setTexture(cardText[i]);

	sf::Image cardBackImg;
	cardBackImg.loadFromFile("Textures/single.jpg");
	sf::Texture cardBackText;
	cardBackText.loadFromImage(cardBackImg);
	sf::Sprite cardBackSpr;
	cardBackSpr.setTexture(cardBackText);

	sf::Image cardFonImg;
	cardFonImg.loadFromFile("Textures/fon.jpg");
	sf::Texture cardFonText;
	cardFonText.loadFromImage(cardFonImg);
	sf::Sprite cardFonSpr;
	cardFonSpr.setTexture(cardFonText);

	sf::Image lineImg;
	lineImg.loadFromFile("Textures/line.jpg");
	sf::Texture lineText;
	lineText.loadFromImage(lineImg);
	sf::Sprite lineSpr;
	lineSpr.setTexture(lineText);
	lineSpr.setPosition(640, 0);

	sf::Image winPlImg;
	winPlImg.loadFromFile("Textures/winPl.jpg");
	sf::Texture winPlTex;
	winPlTex.loadFromImage(winPlImg);
	sf::Sprite winPlSpr;
	winPlSpr.setTexture(winPlTex);
	winPlSpr.setPosition(350, 360);

	sf::Image winBotImg;
	winBotImg.loadFromFile("Textures/winBot.jpg");
	sf::Texture winBotTex;
	winBotTex.loadFromImage(winBotImg);
	sf::Sprite winBotSpr;
	winBotSpr.setTexture(winBotTex);
	winBotSpr.setPosition(350, 360);

	bool numb[ALL];
	sf::Sprite deck[ALL];
	for (int i = 0; i < ALL; i++) {
		deck[i].setTexture(cardText[i % 13]);
		int coordX = rand() % 1000 + 100, coordY = rand() % 600 + 200;
		deck[i].setPosition(coordX, coordY);
		numb[i] = true;
	}

	std::vector<sf::Sprite> columns(10);
	std::vector<int> columnsCount(10);
	for (unsigned int i = 0; i < columns.size(); i++) {
		cardFonSpr.setPosition(50 + i * 125, 10);
		columns[i] = cardFonSpr;
		columnsCount[i] = 13;
	}

	bool isMove = false, isChange = false, isGame = true;	// Variable 'isMove' used to define click on card.
	float dX = 0;	// The change of coordinate x.
	float dY = 0;	// The change of coordinate y.
	int changeX = 0, columnBot = 0, columnPlayer = 0;

	sf::Clock clock;	// Create clock to note time.
	float CurrentFrame = 0;

	while (window.isOpen()) {
		float time = clock.getElapsedTime().asMicroseconds();	// Get time in milliseconds.
		clock.restart();	// Restart clock.
		time = time / 800;	// Set game frame rate.
		CurrentFrame += 0.0025 * time;

		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);	// Get global mouse coooordinates.
		sf::Vector2f pos = window.mapPixelToCoords(pixelPos);	// Get local mouse coordinates.
		sf::Vector2f temppos = window.mapPixelToCoords(pixelPos);
		sf::Vector2f oldPos, newPos;

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {			// If user pressed 'Esc' then close program.
				showMainMenu = true;
				return true;
			}

			// Keep track mouse button and calculate coordinates difference.
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.key.code == sf::Mouse::Left) {
					for (int i = 0; i < ALL; i++) {
						if (deck[i].getGlobalBounds().contains(pos.x, pos.y)) {
							dX = pos.x - deck[i].getPosition().x;
							dY = pos.y - deck[i].getPosition().y;
							isMove = true;
							changeX = i;
							oldPos = deck[i].getPosition();
						}
					}
				}
			}
			if (event.type == sf::Event::MouseButtonReleased)
				if (event.key.code == sf::Mouse::Left)
					isMove = false;

		}
		if (isGame) {
			if (CurrentFrame > 1) {
				CurrentFrame -= 1;
				int index = rand() % 130;
				while ((!numb[index]) && (index != changeX)) {
					index = rand() % 130;
				}
				for (unsigned int i = 5; i < columns.size(); i++) {
					temppos = columns[i].getPosition();
					if (columnsCount[i] == index % 13 + 1) {
						deck[index].setPosition(temppos.x, temppos.y);
						numb[index] = false;
						columns[i] = deck[index];
						columnsCount[i] = index % 13;
						break;
					}
					else {
						int coordX = rand() % 1000 + 100, coordY = rand() % 600 + 200;
						deck[index].setPosition(coordX, coordY);
					}
				}
			}

			// Check that user cannot move card beyond the table.
			if (isMove) {
				deck[changeX].setPosition(pos.x - dX, pos.y - dY);
				newPos = deck[changeX].getPosition();
				oldPos = deck[changeX].getPosition();
				if ((deck[changeX].getPosition().x < 0) && (deck[changeX].getPosition().y < 0))
					deck[changeX].setPosition(0, 0);
				else if ((deck[changeX].getPosition().x < 0) && (deck[changeX].getPosition().y > 820))
					deck[changeX].setPosition(0, 820);
				else if ((deck[changeX].getPosition().x > 1200) && (deck[changeX].getPosition().y > 820))
					deck[changeX].setPosition(1200, 820);
				else if ((deck[changeX].getPosition().x > 1200) && (deck[changeX].getPosition().y < 0))
					deck[changeX].setPosition(1200, 0);
				else if (deck[changeX].getPosition().x < 0)
					deck[changeX].setPosition(0, newPos.y);
				else if (deck[changeX].getPosition().x > 1200)
					deck[changeX].setPosition(1200, newPos.y);
				else if (deck[changeX].getPosition().y < 0)
					deck[changeX].setPosition(newPos.x, 0);
				else if (deck[changeX].getPosition().y > 820)
					deck[changeX].setPosition(newPos.x, 820);
				newPos = deck[changeX].getPosition();
				isChange = false;
			} // Check if user moved card to finish card position.
			else if (!isChange) {
				for (unsigned int i = 0; i < columns.size(); i++) {
					pos.x = columns[i].getPosition().x;
					pos.y = columns[i].getPosition().y;
					if ((deck[changeX].getPosition().x < pos.x + 75) && (deck[changeX].getPosition().y < pos.y + 50)) {
						if (columnsCount[i] == changeX % 13 + 1) {
							deck[changeX].setPosition(pos.x, pos.y);
							numb[changeX] = false;
							columns[i] = deck[changeX];
							columnsCount[i] = changeX % 13;
							isChange = true;
							break;
						}
						else {
							int coordX = rand() % 1000 + 100, coordY = rand() % 600 + 200;
							deck[changeX].setPosition(coordX, coordY);
							isChange = true;
							break;
						}
					}
				}
			}
		}

		window.clear(sf::Color(0, 128, 0));

		window.draw(lineSpr);
		columnBot = 0; 
		columnPlayer = 0;
		for (unsigned int i = 0; i < columns.size(); i++) {
			if ((i < 5) && (columnsCount[i] == 0))
				columnPlayer++;
			if ((i >= 5) && (columnsCount[i] == 0))
				columnBot++;
			window.draw(columns[i]);
		}
		for (int i = 0; i < ALL; i++) {
			if (numb[i])
				window.draw(deck[i]);
		}

		if (columnPlayer == 5) {
			isGame = false;
			window.draw(winPlSpr);
		}
		if (columnBot == 5) {
			isGame = false;
			window.draw(winBotSpr);
		}

		window.display();
	}

	return 0;
}

/// Start/restart game function.
void gameRunning(sf::RenderWindow &window, int &numberLevel, bool &showMainMenu) {
	if (startGame(window, numberLevel, showMainMenu)) {
		gameRunning(window, numberLevel, showMainMenu);
	}
}

/// Main function.
int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 920), "CardGame", sf::Style::Close);
	window.setFramerateLimit(60);
	int numberLevel = 1;			// Number of game level.
	bool showMainMenu = true;
	gameRunning(window, numberLevel, showMainMenu);			// Start game.
	return 0;
}