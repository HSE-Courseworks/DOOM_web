#pragma once
#include "ConsoleController.h"
#include "MapController.h"
#include "Player.h"

#include <chrono>

class Raycasting {
public:
	Raycasting();
	Raycasting(const Player& _player);
	void Raycast();
private:
	Player player;
};