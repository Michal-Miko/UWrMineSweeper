#include "Gui.h"

void Gui::loadSettings() {
	GDifficulty d = (GDifficulty)difficulty->getSelectedItemIndex();
	Tile::setTheme((TTheme)theme->getSelectedItemIndex());
	state->setDiff(d);

	if (d == GDifficulty::custom) {
		unsigned w, h;
		std::istringstream iss;
		iss.str(width->getText().toAnsiString());
		if (!(iss >> w))
			w = 24;

		iss.str(height->getText().toAnsiString());
		iss.clear();
		if (!(iss >> h))
			h = 30;

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
		if (difficulty->getSelectedItemIndex() == (int)GDifficulty::custom)
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
	theme->setSelectedItemByIndex((int)Tile::theme);

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
		winSize = window->getSize();

		if (!resizing) {
			resize();
		}
		else
			resizing = false;

	}

	if (state->getGState() == GState::paused)
		return;

	if (e.type == Event::MouseButtonPressed &&
		e.mouseButton.y >= 40) {
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
			40.0f,
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

	std::stringstream ss;

	// Flags
	ss << state->getFlagCount() << "/" << state->getMineCount();
	mines->setText(ss.str());
	ss.str(std::string());
	// TODO: Fix total mine count after resizing board

	// Time
	sf::Time t = state->getTimeElapsed();
	int ms = t.asMilliseconds() % 1000;
	int s = t.asSeconds();
	ss << s << "." << ms;
	time->setText(ss.str());

	// Pause
	if (state->getGState() == GState::victory) {
		gameOverText->setText("Victory!");
		gameOver->setVisible("True");
		//state->revealMines();
		state->setGState(GState::paused);
		endTime->setText(ss.str());
	}
	else if (state->getGState() == GState::loss) {
		gameOverText->setText("Game Over");
		gameOver->setVisible("True");
		state->revealMines();
		state->setGState(GState::paused);
		endTime->setText(ss.str());
	}

}

void Gui::resize() {
	Vector2u size = state->getSize();

	if ((winSize.y - 40) % size.y != 0)
		winSize.y -= (winSize.y - 40) % size.y;

	float w = winSize.x / (float)size.x;
	float h = (winSize.y - 40) / (float)size.y;

	float upp = size.y * 16 / (winSize.y - 40.0f);
	float offset = upp * 40;

	sf::View v1(sf::FloatRect(0, -offset, size.x * 16, size.y * 16 + offset));
	window->setView(v1);

	winSize.x = h * size.x;

	sf::View v2(sf::FloatRect(0, 0, winSize.x, winSize.y));
	gui.setView(v2);

	if (winSize.x % size.x != 0)
		winSize.x -= winSize.x % size.x;

	if (winSize.x < 320) {
		winSize.y += size.y;
		resize();
		return;
	}

	window->setSize(winSize);
	resizing = true;
}

void Gui::draw() {
	gui.draw();
}