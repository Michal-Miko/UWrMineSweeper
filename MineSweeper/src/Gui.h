#pragma once

#include <TGUI/TGUI.hpp>
#include <sstream>
#include "MineSweeper.h"
#include "Tiles.h"
#include "Utils.h"

typedef tgui::Gui TGui;
using tgui::Panel;
using tgui::Label;
using tgui::Button;
using tgui::ChildWindow;
using tgui::ComboBox;
using tgui::TextBox;

class Gui {
private:
	MineSweeper* state;
	sf::RenderWindow* window;
	Vector2u winSize;
	bool resizing;
	TGui gui;

	// Top panel
	Panel::Ptr topPanel;
	Label::Ptr mines;
	Label::Ptr time;
	Button::Ptr reset;
	Button::Ptr openSettings;

	// Settings
	ChildWindow::Ptr settings;
	ComboBox::Ptr difficulty;
	ComboBox::Ptr theme;
	Panel::Ptr customPanel;
	TextBox::Ptr width;
	TextBox::Ptr height;
	Button::Ptr ok;
	Button::Ptr cancel;
	Button::Ptr apply;

	void loadSettings();

	// GameOver
	ChildWindow::Ptr gameOver;
	Label::Ptr gameOverText;
	Label::Ptr endTime;

public:
	Gui(MineSweeper* state, sf::RenderWindow* target, std::string guiPath);
	void handleEvents(const sf::Event& e);
	void setView(sf::View view);
	void update();
	void resize();
	void draw();
};