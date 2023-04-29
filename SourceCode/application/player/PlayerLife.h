#pragma once
#include "Player.h"
#include "Sprite.h"
#include <DirectXMath.h>

class PlayerLife
{
public:
	void Initialize();
	void Update(int playerLife);
	void Draw();

private:
	std::unique_ptr<Player> player;

	std::unique_ptr<Sprite> lifeSprite[8]{};

	int life = 0;
};

