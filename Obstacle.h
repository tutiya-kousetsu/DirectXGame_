#pragma once
#include "GameObject.h"
#include <DirectXMath.h>
using namespace DirectX;

class Obstacle : public GameObject
{
public:
	using GameObject::GameObject;

	Obstacle();

	void Updata() override;

	void Draw() override;

private:
	XMVECTOR position;
};

