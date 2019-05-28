#include "Gui.h"

void Gui::loadSettings() {
	GDifficulty d = (GDifficulty)difficulty->getSelectedItemIndex();
	state->setDiff(d);

	if (d == GDifficulty::custom) {
		unsigned w, h;
		std::istringstream iss;
		iss.str(width->getText());
		iss >> w;
		iss.str(height->getText());
		iss.clear();
		iss >> h;
		state->setSize(Vector2u(w, h));
	}

	state->reset();
	resize();
}

Gui::Gui(MineSweeper * state, sf::RenderWindow* target, std::string guiPath) {
	gui.setTarget(*target);
	gui.loadWidgetsFromFile(guiPath);

	// Importing gui pointers
	this->state = state;
	this->window = target;
	winSize = target->getSize();

	// Top Panel
	topPanel = gui.get<Panel>("TopPanel");
	mines = gui.get<Label>("Mines");
	time = gui.get<Label>("Time");
	reset = topPanel->get<Button>("Reset");
	openSettings = topPanel->get<Button>("OpenSettings");

	// Settings
	settings = gui.get<ChildWindow>("Settings");
	difficulty = settings->get<ComboBox>("Difficulty");
	theme = settings->get<ComboBox>("Theme");
	customPanel = settings->get<Panel>("CustomPanel");
	width = settings->get<TextBox>("Width");
	height = settings->get<TextBox>("Height");
	ok = settings->get<Button>("OK");
	cancel = settings->get<Button>("Cancel");
	apply = settings->get<Button>("Apply");

	// GameOver
	gameOver = gui.get<ChildWindow>("GameOver");
	gameOverText = gameOver->get<Label>("Text");
	endTime = gameOver->get<Label>("Score");

	// Reset button
	reset->setPosition(
		tgui::bindLeft(openSettings) - tgui::bindSize(reset).x - 8,
		"25%"
	);
	reset->connect("pressed", [&](MineSweeper* s) {
		s->reset();
		gameOver->setVisible(false);
	}, state);

	// Settings button
	openSettings->connect("pressed", [&]() {
		settings->setVisible(!settings->isVisible());
	});

	// Settings window
	settings->connect("closed", [&]() {
		settings->setVisible(false);
	});
	difficulty->connect("itemselected", [&]() {
		if (difficulty->getSelectedItem().toAnsiString() == "custom")
			customPanel->setVisible(true);
		else customPanel->setVisible(false);
	});
	ok->connect("pressed", [&]() {
		loadSettings();
		settings->setVisible(false);
	});
	cancel->connect("pressed", [&]() {
		settings->setVisible(false);
	});
	apply->connect("pressed", [&]() {
		loadSettings();
	});
	difficulty->setSelectedItemByIndex((int)state->getDiff());

	// GameOver window
	gameOver->connect("closed", [&]() {
		gameOver->setVisible(false);
	});
}

void Gui::handleEvents(const sf::Event & e) {
	if (gui.handleEvent(e))
		return;

	if (e.type == Event::Resized) {
		std::cout << "Window resized:\n";
		std::printf(
			"[%u,%u] => [%u,%u])\n",
			winSize.x,
			winSize.y,
			window->getSize().x,
			window->getSize().y
		);

		if (!resizing) {
			resize();
		}
		else {
			resizing = false;
			winSize = window->getSize();
		}
	}

	if (state->getGState() == GState::paused)
		return;

	if (e.type == Event::MouseButtonPressed &&
		e.mouseButton.y >= winSize.y*0.06f) {
		// Position
		unsigned mx = e.mouseButton.x;
		unsigned my = e.mouseButton.y;
		Vector2u size = state->getSize();

		// Mapping to block pos
		mx = (unsigned)map(
			(float)mx,
			0.0f,
			(float)winSize.x,
			0.0f,
			(float)size.x
		);
		my = (unsigned)map(
			(float)my,
			winSize.y*0.06f,
			(float)winSize.y,
			0.0f,
			(float)size.y
		);

		state->clickOnTile(Vector2u(mx, my), e.mouseButton.button);
	}
}

void Gui::setView(sf::View view) {
	gui.setView(view);
}

void Gui::update() {
	// Check for victory
	if (state->getGState() == GState::running)
		state->checkVictory();

	// Pause
	if (state->getGState() == GState::victory) {
		gameOverText->setText("Victory!");
		gameOver->setVisible("True");
		//state->revealMines();
		state->setGState(GState::paused);
		endTime->setText(time->getText());
	}
	else if (state->getGState() == GState::loss) {
		gameOverText->setText("Game Over");
		gameOver->setVisible("True");
		state->revealMines();
		state->setGState(GState::paused);
		endTime->setText(time->getText());
	}

	std::stringstream ss;

	// Flags
	ss << state->getFlagCount() << "/" << state->getMineCount();
	mines->setText(ss.str());
	ss.str(std::string());

	// Time
	sf::Time t = state->getTimeElapsed();
	int ms = t.asMilliseconds() % 1000;
	int s = t.asSeconds();
	ss << s << "." << ms;
	time->setText(ss.str());
}

void Gui::resize() {
	winSize = window->getSize();
	Vector2u size = state->getSize();

	sf::View v1(sf::FloatRect(0, 0, size.x * 16, size.y * 16));
	v1.setViewport(sf::FloatRect(0, 0.06, 1, 0.94));
	window->setView(v1);

	float w = winSize.x / size.x;
	float h = winSize.y / size.y;

	winSize = Vector2u(h * size.x, (unsigned)(winSize.y * 100 / 94));
	sf::View v2(sf::FloatRect(0, 0, winSize.x, winSize.y));
	gui.setView(v2);

	if (abs(w - h) > FLT_EPSILON) {
		window->setSize(winSize);
		resizing = true;
	}
}

void Gui::draw() {
	gui.draw();
}