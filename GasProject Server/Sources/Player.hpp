#pragma once

#include <string>

#include <PlayerCommand.hpp>
#include <World/Camera/Camera.hpp>

#include <Shared/Types.hpp>
#include <Shared/ThreadSafeQueue.hpp>

class Server;
class Game;
class NetworkController;
struct Connection;
struct ServerCommand;

class Control;

class Player {
friend NetworkController;
friend Server;

public:
	explicit Player(std::string ckey);

	void SetConnection(sptr<Connection> &connection);

    /// Client interface
    void UpdateServerList();
    void JoinToGame(Game *game);
    void ChatMessage(std::string &message);

    void Move(uf::Direction);
    void ClickObject(uint id);
	void Drop();
	void Build();
	void Ghost();
    ///

    void Update();
    void SendGraphicsUpdates(sf::Time timeElapsed);

	void OpenWindow(const char *layout);

    std::string GetCKey() { return ckey; }

	void Suspend();
    void SetControl(Control *control);
	void SetCamera(Camera *camera);

	sptr<Connection> GetConnection();
	Control *GetControl();
	Camera *GetCamera();
	bool IsConnected();

    void AddCommandToClient(ServerCommand *);

private:
	std::string ckey;

	//bool connected;
	Game *game;

	Control *control;
	uptr<Camera> camera;

	wptr<Connection> connection;
	uf::ThreadSafeQueue<PlayerCommand *> actions;

	bool atmosOverlayToggled;
};
