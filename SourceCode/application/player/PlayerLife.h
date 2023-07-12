#pragma once
#include "Sprite.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include <memory>
class PlayerLife
{
public:
	void Initialize();
	void Draw(int life);

private:

	DirectX::XMFLOAT2 pos[9]{};

	std::unique_ptr<Sprite> lifeSp[9]{};

	bool disFlag = false;

	int life = 0;
};