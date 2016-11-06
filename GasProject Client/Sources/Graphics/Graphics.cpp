#include "Client.hpp"
#include "Window.hpp"
#include "TileGrid/TileGrid.hpp"
#include "UI/UI.hpp"
#include "State.hpp"

Block::Block(TileGrid *tileGrid) : tileGrid(tileGrid),
								   tiles(tileGrid->BlockSize,
								   vector<Tile *>(tileGrid->BlockSize)) {
	for (auto &vect : tiles)
		for (auto &tile : vect)
			tile = new Tile(this);
}

void Window::loadTextures() {
	Texture * human = new Texture(1, "Images/Human.png", 32, textures, 1);
	human->SetInfo(0, 0, true, 1);
}

void Window::Update(sf::Time timeElapsed) {
	sf::Event event;
	State *state = CC::Get()->GetState();
	while (window->pollEvent(event)) {
		ui->HandleEvent(event);
		if (state)
			state->HandleEvent(event);
		if (event.type == sf::Event::Closed)
			window->close();
	}

	window->resetGLStates();
	window->clear(sf::Color::Black);
    if (state) {
        state->DrawTileGrid(window.get(), tileGrid.get());
        state->DrawUI(window.get(), timeElapsed);
    }
	sizeTile = min(width, height) / 15;
	window->display();
	sf::sleep(sf::milliseconds(10));
}

void MenuLoginState::DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const { }
void MenuGameListState::DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const { }
void GameLobbyState::DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const { }
void GameProcessState::DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const {
	tileGrid->Draw(render_window);
}

void MenuLoginState::DrawUI(sf::RenderWindow *render_window, sf::Time timeElapsed) const {
	Window *window = CC::Get()->GetWindow();
    AuthUI *authUI = window->GetUI()->GetAuthUI();

    while (authUI->comState != AuthUI::ComState::NOTHING) {
        AuthUI::ServerAnswer answer = window->GetUI()->GetAuthUI()->GetAnswer();
        if (answer.isAnswer) {
            if (authUI->comState == AuthUI::ComState::LOGIN) {
				authUI->comState = AuthUI::ComState::NOTHING;
                if (answer.result) {
                    CC::log << "You logged in succesfully" << endl;
                    CC::Get()->SetState(new MenuGameListState);
                } else {
                    CC::log << "You did not log in" << endl;
                }
            }
            if (authUI->comState == AuthUI::ComState::REGISTRATION) {
				authUI->comState = AuthUI::ComState::NOTHING;
                if (answer.result)
                    CC::log << "You are succesfully registered" << endl;
                else
                    CC::log << "Problems with registration" << endl;
                authUI->openLogin();
            }
        }
		if (authUI->comState != AuthUI::ComState::NOTHING) {
			sf::sleep(sf::seconds(0.5f));
			CC::log << "Waiting for network" << endl;
			render_window->display();
		}
    }

    window->GetUI()->Lock();
    window->GetUI()->Update(timeElapsed);
    window->GetUI()->Draw(render_window);
    window->GetUI()->Unlock();
}
void MenuGameListState::DrawUI(sf::RenderWindow *render_window, sf::Time timeElapsed) const {
    Window *window = CC::Get()->GetWindow();
    window->GetUI()->Lock();
    window->GetUI()->Update(timeElapsed);
    window->GetUI()->Draw(render_window);
    window->GetUI()->Unlock();
}
void GameLobbyState::DrawUI(sf::RenderWindow *render_window, sf::Time timeElapsed) const { }
void GameProcessState::DrawUI(sf::RenderWindow *render_window, sf::Time timeElapsed) const { }

void MenuLoginState::HandleEvent(sf::Event event) const {
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab)
		CC::Get()->GetWindow()->GetUI()->GetAuthUI()->ChangeFocus();
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
		CC::Get()->GetWindow()->GetUI()->GetAuthUI()->AccountDataEnter();
}
void MenuGameListState::HandleEvent(sf::Event event) const { }
void GameLobbyState::HandleEvent(sf::Event event) const { }
void GameProcessState::HandleEvent(sf::Event event) const { }