#include <SFML/Graphics.hpp>
#include "MineSweeper.h"
#include "Gui.h"

using std::string;

using namespace sf;

int main() { // NOLINT(bugprone-exception-escape)
	Vector2u winSize(400, 677);

	// paths
	const string assets = "../../assets/";

	// Create the window of the application
	RenderWindow window(
		VideoMode(winSize.x, winSize.y),
		"MineSweeper",
		Style::Titlebar | Style::Close | Style::Resize
	);

	window.setFramerateLimit(60);

	// Set icon
	Image icon;
	icon.loadFromFile(assets + "icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	MineSweeper state(assets, GDifficulty::easy);

	Gui gui(&state, &window, assets + "UI.txt");
	gui.resize();

	//  Requires Utils.h
	//Fps fpsCounter;

	// Window loop
	while (window.isOpen()) {
		Event event{};
		// check all the window's events
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();

			gui.handleEvents(event);
		}

		gui.update();

		window.clear(Color::White);
		window.draw(state.tilemap);

		gui.draw();

		window.display();

		/* FPS
		fpsCounter.update();
		std::ostringstream ss;
		ss << "FPS: " << fpsCounter.getFps();
		window.setTitle(ss.str());
		*/
	}

	return EXIT_SUCCESS;
}
