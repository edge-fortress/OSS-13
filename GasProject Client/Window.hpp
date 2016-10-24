#pragma once

#include <memory>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "TileGrid.hpp"
#include "UI.hpp"

class ClientController;

using sf::RenderWindow;

class Window {
private:
	uptr<TileGrid> tileGrid;
	uptr<UI> ui;

	uptr<RenderWindow> window;
	ClientController *controller;
	int width, height;

	const int req_FPS = 100;
	sf::Clock clock;
	int cur_FPS;

	bool fps_exceed() {
		if (clock.getElapsedTime() >= sf::milliseconds(100)) {
			cur_FPS = 0;
			clock.restart();
		}
		if (cur_FPS >= req_FPS / 10) return true;
		return false;
	}

public:
	Window(ClientController *controller) : tileGrid(new TileGrid),
										   controller(controller), 
										   cur_FPS(0) {
		sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
		width = static_cast<int>(0.9 * videoMode.width);
		height = static_cast<int>(0.9 * videoMode.height);
		window.reset(new RenderWindow(sf::VideoMode(width, height), "GasProjectClient"));
		window->clear(sf::Color::Black);
		window->display();
		ui.reset(new UI(controller, window.get()));
	}

	Window(const Window &) = delete;
	Window &operator=(const Window &) = delete;
	virtual ~Window() = default;

	void Update(sf::Time);

	bool isOpen() const { return window->isOpen(); }

	UI *GetUI() const { return ui.get(); }
};