#pragma once
#include "Sprite.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include <memory>
class Arrow
{
public:
	void Initialize();

	void Draw(int i);

private:
	DirectX::XMFLOAT2 pos[9]{};
	//Phase
	std::unique_ptr<Sprite> arrow[9]{};

	//イーズイン用frame
	float inFrame = 0.f;
	//イーズアウト用frame
	float outFrame = 0.f;
};