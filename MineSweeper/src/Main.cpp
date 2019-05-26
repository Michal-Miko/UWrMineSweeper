#include <SFML/Graphics.hpp>
#include <iostream>
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
	Vector2u winSize(800, 800);

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
		}

		window.clear();

		window.display();
	}

	return EXIT_SUCCESS;
}