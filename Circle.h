#pragma once
#include "Sprite.h"
#include <memory>
#include <DirectXMath.h>

class Circle
{
public:
	void Initialize();

	void Update();

	void UpSize();

	void DownSize(int32_t time);

	void Draw();
private:
	std::unique_ptr<Sprite> sprite;
	DirectX::XMFLOAT2 size;
	int32_t moveTime = 0.0f;
	float xFrame = 0.0f;
	float yFrame = 0.0f;
};