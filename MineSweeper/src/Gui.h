#pragma once

#include <TGUI/TGUI.hpp>
#include "MineSweeper.h"
#include "Tiles.h"

// ReSharper disable once CppInconsistentNaming
using TGui = tgui::Gui;
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
	Label::Ptr flares;
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

	// GameOver
	ChildWindow::Ptr gameOver;
	Label::Ptr gameOverText;
	Label::Ptr endTime;

	// Read information from settings menu
	void loadSettings();
public:
	Gui(MineSweeper* state, sf::RenderWindow* target, const std::string& guiPath);
	void handleEvents(const Event& e);
	void setView(sf::View view);
	void update() const;
	void resize();
	void draw();
};
