#include "Gui.h"
#include <sstream>
#include <iostream>
#include "Utils.h"

void Gui::loadSettings() {
	const auto d = GDifficulty(difficulty->getSelectedItemIndex());
	Tile::setTheme(TTheme(theme->getSelectedItemIndex()));
	state->diff = d;

	if (d == GDifficulty::custom) {
		unsigned w, h;
		std::istringstream iss;
		iss.str(width->getText().toAnsiString());
		if (!(iss >> w)) {
			w = 24;
			width->setText("24");
		}

		iss.str(height->getText().toAnsiString());
		iss.clear();
		if (!(iss >> h)) {
			h = 30;
			height->setText("30");
		}

		if (w > 100) {
			w = 100;
			width->setText("100");
		}

		if (h > 100) {
			h = 100;
			height->setText("100");
		}

		state->size = Vector2u(w, h);
	}

	state->reset();
	resize();
}

Gui::Gui(MineSweeper* state, sf::RenderWindow* target, const std::string& guiPath) {
	resizing = false;
	gui.setTarget(*target);
	gui.loadWidgetsFromFile(guiPath);

	this->state = state;
	this->window = target;
	winSize = target->getSize();

	// Importing gui pointers
	// Top Panel
	topPanel = gui.get<Panel>("TopPanel");
	mines = gui.get<Label>("Mines");
	flares = gui.get<Label>("Flares");
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
		bindLeft(openSettings) - bindSize(reset).x - 8,
		"25%"
	);
	reset->connect("pressed", [&](MineSweeper* s) {
		s->reset();
		gameOver->setVisible(false);
	}, state);

	// Settings button
	openSettings->connect("pressed", [&]() { settings->setVisible(!settings->isVisible()); });

	// Settings window
	settings->connect("closed", [&]() { settings->setVisible(false); });
	difficulty->connect("itemselected", [&]() {
		if (difficulty->getSelectedItemIndex() == int(GDifficulty::custom))
			customPanel->setVisible(true);
		else customPanel->setVisible(false);
	});
	ok->connect("pressed", [&]() {
		loadSettings();
		settings->setVisible(false);
		gameOver->setVisible(false);
	});
	cancel->connect("pressed", [&]() { settings->setVisible(false); });
	apply->connect("pressed", [&]() {
		loadSettings();
		gameOver->setVisible(false);
	});
	difficulty->setSelectedItemByIndex(int(state->diff));
	theme->setSelectedItemByIndex(int(Tile::theme));

	// GameOver window
	gameOver->connect("closed", [&]() { gameOver->setVisible(false); });
}

void Gui::handleEvents(const Event& e) {
	if (gui.handleEvent(e))
		return;

	if (e.type == Event::KeyPressed && e.key.code == sf::Keyboard::R) {
		state->reset();
		gameOver->setVisible(false);
	}

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

		if (!resizing) { resize(); }
		else
			resizing = false;
	}

	if (state->gState == GState::paused)
		return;

	if (e.type == Event::MouseButtonPressed &&
		e.mouseButton.y >= 40) {
		// Position
		unsigned mx = e.mouseButton.x;
		unsigned my = e.mouseButton.y;
		const auto size = state->size;

		// Mapping to block pos
		mx = unsigned(map(
			float(mx),
			0.0f,
			float(winSize.x),
			0.0f,
			float(size.x)
		));
		my = unsigned(map(
			float(my),
			40.0f,
			float(winSize.y),
			0.0f,
			float(size.y)
		));

		state->clickOnTile(Vector2u(mx, my), e.mouseButton.button);
	}
}

void Gui::setView(sf::View view) { gui.setView(view); }

void Gui::update() const {
	// Check for victory
	if (state->gState == GState::running)
		state->checkVictory();

	std::stringstream ss;

	// Flares
	ss << state->flareCount;
	flares->setText(ss.str());
	ss.str(std::string());

	// Flags
	ss << state->flagCount << "/" << state->mineCount;
	mines->setText(ss.str());
	ss.str(std::string());

	// Time
	if (state->gState == GState::running) {
		const auto t = state->clock.getElapsedTime();
		const auto ms = t.asMilliseconds() % 1000;
		const auto s = int(t.asSeconds());
		ss << s << "." << ms;
		time->setText(ss.str());
	}

	// Pause
	if (state->gState == GState::victory) {
		gameOverText->setText("Victory!");
		gameOver->setVisible("True");
		//state->revealMines();
		state->gState = GState::paused;
		endTime->setText(time->getText());
	}
	else if (state->gState == GState::loss) {
		gameOverText->setText("Game Over");
		gameOver->setVisible("True");
		state->revealMines();
		state->gState = GState::paused;
		endTime->setText(time->getText());
	}
}

void Gui::resize() {
	const auto size = state->size;

	if ((winSize.y - 40) % size.y != 0)
		winSize.y -= (winSize.y - 40) % size.y;

	const auto h = float(winSize.y - 40) / float(size.y);

	const auto upp = float(size.y) * 16 / (winSize.y - 40.0f);
	const auto offset = upp * 40;

	const sf::View v1(sf::FloatRect(0, -offset, float(size.x) * 16, float(size.y) * 16 + offset));
	window->setView(v1);

	winSize.x = unsigned(h * size.x);

	const sf::View v2(sf::FloatRect(0, 0, float(winSize.x), float(winSize.y)));
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

void Gui::draw() { gui.draw(); }
