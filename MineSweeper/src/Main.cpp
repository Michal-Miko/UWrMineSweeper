#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "Tilemap.h"
#include "GameState.h"
#include "Utils.h"

using std::string;
using std::vector;

using namespace sf;

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main() {
	Vector2u winSize(20 * 20, 36 * 20);

	// paths
	string assets = "../../game/assets/";

	// Create the window of the application
	RenderWindow window(
		VideoMode(winSize.x, winSize.y),
		"MineSweeper",
		Style::Titlebar | Style::Close | Style::Resize
	);

	/*
	// Set icon
	Image icon;
	icon.loadFromFile(assets + "icons/i16.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	*/

	GameState state("../../assets/", GDifficulty::veryHard);
	Vector2u size = state.getSize();
	window.setSize(Vector2u(size.x * 20, size.y * 20));

	View view(FloatRect(0, 0, size.x * 16, size.y * 16));
	window.setView(view);

	FPS fpsCounter;

	// Window loop
	while (window.isOpen()) {
		Event event;
		// check all the window's events
		while (window.pollEvent(event)) {
			// "close requested" event: we close the window
			if (event.type == Event::Closed)
				window.close();

			// Window size changed
			if (event.type == Event::Resized) {
				std::cout << "Window resized:\n";
				std::printf("[%u,%u] => [%u,%u])\n", winSize.x, winSize.y, window.getSize().x, window.getSize().y);
			}

			if (event.type == Event::KeyPressed)
				if (event.key.code == sf::Keyboard::R)
					state.reset();


			if (state.getGState() == GState::running)
				state.handleEvents(event, window);
		}

		window.clear(Color::White);

		window.draw(state.getBG());
		window.draw(state.getFG());

		window.display();

		fpsCounter.update();
		std::ostringstream ss;
		ss << "FPS: " << fpsCounter.getFPS();

		window.setTitle(ss.str());

	}

	return EXIT_SUCCESS;
}