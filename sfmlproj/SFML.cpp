#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

sf::Color HSVToRGB(float h, float s, float v) {
	float hPrime = h / 60.0f;
	unsigned int hIndex = unsigned int(hPrime) % 6;
	float chroma = s * v;
	float min = (v - chroma);
	float x = chroma * (1.0f - abs(fmod(hPrime, 2.0f) - 1.0f));
	float outRGB[6][3] = {

		{chroma, x, 0.0f},
		{x, chroma, 0.0f},
		{0.0f, chroma, x},
		{0.0f, x, chroma},
		{x, 0.0f, chroma},
		{chroma, 0.0f, x}

	};
	float rF = (outRGB[hIndex][0] + min);
	float gF = (outRGB[hIndex][1] + min);
	float bF = (outRGB[hIndex][2] + min);
	rF *= 255;
	gF *= 255;
	bF *= 255;
	std::uint8_t rI = std::uint8_t(rF);
	std::uint8_t gI = std::uint8_t(gF);
	std::uint8_t bI = std::uint8_t(bF);
	return sf::Color(rI, gI, bI);
}
void PollEvents(sf::RenderWindow& window) {
	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			window.close();
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
				window.close();
			}
		}
	}
}

int main() {
	unsigned int width = 405;
	unsigned int height = 450;
	const float tilesize = 20.0f;
	sf::Vector2u windowSize = { width, height };
	sf::VideoMode videomode = sf::VideoMode(windowSize);
	std::string title = "SFMLL";
	sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({ width, height }), title);
	window.setFramerateLimit(60);

	sf::Texture grassTex;
	if (!grassTex.loadFromFile("Textures/jungle.jpg")) {
		std::cerr << "Failed to load block texture warning!!!\n";
	}
	sf::Texture grassTex2;
	if (!grassTex2.loadFromFile("Textures/grass.png")) std::cerr << "Failed to load grass texture!!\n";

	sf::Texture stoneTex;
	if (!stoneTex.loadFromFile("Textures/stone.jpg")) std::cerr << "Failed to load STONE texture!!\n";

	sf::Texture diamondTex;
	if (!diamondTex.loadFromFile("Textures/diamond.jpg")) std::cerr << "Failed to load diamond texture\n";

	sf::Texture playerTexr;
	if (!playerTexr.loadFromFile("Textures/SteveRight.png")) std::cerr << "Failed to load ssss texture!\n";

	sf::Texture playerTexl;
	if (!playerTexl.loadFromFile("Textures/SteveLeft.png")) std::cerr << "Failed to load ssss texture!\n";

	sf::Texture playerTex;
	if (!playerTex.loadFromFile("Textures/Steve.png")) std::cerr << "Failed to load steves texture!\n";

	sf::Font font;
	if (!font.openFromFile("Fonts/arial.ttf")) std::cout << "Failed to load texture texture AAAA!!\n";
	float playerGravity = 0.2f;
	sf::Vector2f velocity = { 0.0f, 0.0f };
	std::vector<std::string> Design = {
		"::::::::::::::::::::",
		":                  :",
		":                  :",
		":                  :",
		":                  :",
		":                  :",
		":  &&&&            :",
		":                  :",
		":                  :",
		":                  :",
		":                  :",
		":                  :",
		":                  :",
		":      %%%         :",
		":                  :",
		":                  :",
		":                  :",
		":                  :",
		":                  :",
		":    ****          :",
		":                  :",
		"::::::::::::::::::::"
	};
	sf::Text winTex(font);
	std::vector<sf::Sprite> Blocks;

	for (std::size_t i = 0; i < Design.size(); i++) {
		for (std::size_t j = 0; j < Design[i].size(); j++) {
			if (Design[i][j] == ':') {
				sf::Sprite grass(grassTex);
				grass.setScale({ tilesize / static_cast<float>(grassTex.getSize().x) + 0.01f, tilesize / static_cast<float>(grassTex.getSize().y) + 0.01f });
				grass.setPosition({ j * tilesize, i * tilesize });
				
				Blocks.push_back(grass);
			}
			if (Design[i][j] == '*') {
				sf::Sprite grass2(grassTex2);
				grass2.setScale({ tilesize / static_cast<float>(grassTex2.getSize().x) + 0.01f, tilesize / static_cast<float>(grassTex2.getSize().y) + 0.01f });
				grass2.setPosition({ j * tilesize, i * tilesize });
				Blocks.push_back(grass2);

			}
			if (Design[i][j] == '%') {
				sf::Sprite stone(stoneTex);
				stone.setScale({ tilesize / static_cast<float>(stoneTex.getSize().x) + 0.01f, tilesize / static_cast<float>(stoneTex.getSize().y) + 0.01f });
				stone.setPosition({ j * tilesize, i * tilesize });
				Blocks.push_back(stone);
			}
			if (Design[i][j] == '&') {
				sf::Sprite diamond(diamondTex);
				diamond.setScale({ tilesize / static_cast<float>(diamondTex.getSize().x) + 0.01f, tilesize / static_cast<float>(diamondTex.getSize().y) + 0.01f });
				diamond.setPosition({ j * tilesize, i * tilesize });
				Blocks.push_back(diamond);
			}
		}
	}

	sf::Sprite steve(playerTex);
	steve.setPosition({ width / 2.0f, height / 2.0f });
	steve.setScale({ 0.5f, 0.5f });

	

	while (window.isOpen()) {


		//Updating

		PollEvents(window);

		std::cout << steve.getPosition().x << " , " << steve.getPosition().y << '\n';

		float speedX = 0.0f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
			speedX = -3.0f;
			steve.setTexture(playerTexl, true);
		} 
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
			speedX = 3.0f;
			steve.setTexture(playerTexr, true);
		} 
		if (speedX == 0.0f) {
			steve.setTexture(playerTex, true);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space)) {
			velocity.y = -3.0f;
		} 

		steve.move({ speedX, 0.0f});

		for (std::size_t i = 0; i < Blocks.size(); i++) {
			if (steve.getGlobalBounds().findIntersection(Blocks[i].getGlobalBounds())) {
				if (speedX > 0.0f) {
					steve.setPosition({ Blocks[i].getPosition().x - steve.getGlobalBounds().size.x, steve.getPosition().y});
				}
				else if (speedX < 0.0f) {
					steve.setPosition({ Blocks[i].getPosition().x + Blocks[i].getGlobalBounds().size.x, steve.getPosition().y});
				}
			}
		}
		velocity.y += playerGravity;
		steve.move({ 0.0f, velocity.y });

		for (std::size_t i = 0; i < Blocks.size(); i++) {
			if (steve.getGlobalBounds().findIntersection(Blocks[i].getGlobalBounds())) {
				if (velocity.y > 0.0f) {

					velocity.y = 0.0f;
					steve.setPosition({ steve.getPosition().x, Blocks[i].getPosition().y - steve.getGlobalBounds().size.y });
				}
				else if (velocity.y < 0.0f) {
					velocity.y = 0.0f;
					steve.setPosition({ steve.getPosition().x, Blocks[i].getPosition().y + Blocks[i].getGlobalBounds().size.y });
				}
			}
			
		}
		if (steve.getPosition().x <= 84.0f && steve.getPosition().y <= 48) {
			winTex.setString("You win Musor Drop!!!");
			winTex.setScale({ 1.0f, 1.0f });
			winTex.setFillColor(sf::Color::White);
			winTex.setOutlineThickness(2.0f);
			winTex.setOutlineColor(sf::Color::Green);
			winTex.setPosition({ width / 2.0f - 150, height / 2.0f });
		}
			
		

		//Render

		window.clear(sf::Color::Black);

		// Draw
		
		for (const auto& block : Blocks) {
			window.draw(block);
		}
		window.draw(steve);
		window.draw(winTex);

		// Display screen

		window.display();
	}
	return 0;
}