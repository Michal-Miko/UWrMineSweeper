#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "MineSweeper.h"
#include "Gui.h"

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
	Vector2u winSize(400, 677);

	// paths
	string assets = "../../game/assets/";

	// Create the window of the application
	RenderWindow window(
		VideoMode(winSize.x, winSize.y),
		"MineSweeper",
		Style::Titlebar | Style::Close | Style::Resize
	);

	window.setFramerateLimit(60);

	/*
	// Set icon
	Image icon;
	icon.loadFromFile(assets + "icons/i16.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	*/

	MineSweeper state("../../assets/", GDifficulty::easy);
	Vector2u size = state.size;

	Gui gui(&state, &window, "../../assets/UI.txt");
	gui.resize();

	FPS fpsCounter;

	// Window loop
	while (window.isOpen()) {
		Event event;
		// check all the window's events
		while (window.pollEvent(event)) {
			// "close requested" event: we close the window
			if (event.type == Event::Closed)
				window.close();

			gui.handleEvents(event);
		}

		gui.update();

		window.clear(Color::White);
		window.draw(state.tilemap);

		gui.draw();

		window.display();

		fpsCounter.update();
		std::ostringstream ss;
		ss << "FPS: " << fpsCounter.getFPS();

		window.setTitle(ss.str());

	}

	return EXIT_SUCCESS;
}