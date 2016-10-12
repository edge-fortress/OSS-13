#pragma once

#include "State.hpp"

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

class UI {
private:
	sfg::Desktop *desktop;
	sfg::SFGUI m_sfgui;
	sfg::Entry::Ptr login_entry, passw_entry, new_login_entry, new_passw_entry;
	sfg::Window::Ptr reg_window;

	void run_login();
	void run_reg();

public:
	UI() { run_login(); }
	UI(const UI &) = delete;
	UI &operator=(const UI &) = delete;
	virtual ~UI() = default;

	void Enter();
	void Registration();
	void HandleEvent(sf::Event event);

	friend void MenuLoginState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const;
	friend void MenuServerListState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const;
	friend void GameLobbyState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const;
	friend void GameProcessState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const;
};