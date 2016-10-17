#include <list>
#include <SFML/Network.hpp>

#include "Server.hpp"
#include "network.hpp"
#include "World.hpp"

#include <net_const.hpp>

Game::Game(Server *server) : server(server),
						     world(new World()) {

}

Server::Server() {
	Network::Initialize(PORT);
	Network::WIP_Wait();
	games.push_back(uptr<Game>(new Game(this)));
}

int main() {
	Server server;

	return 0;
}