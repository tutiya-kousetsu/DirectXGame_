#pragma once

#include "GameObject.h"
#include <DirectXMath.h>
using namespace DirectX;

class Floor : public GameObject
{
public:
	using GameObject::GameObject;

	Floor();

	void Updata() override;

	void Draw() override;

private:
	XMVECTOR position;
};