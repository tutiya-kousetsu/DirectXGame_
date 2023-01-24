#pragma once
#include "GameObject.h"
#include <DirectXMath.h>
using namespace DirectX;

class Obstacle : public GameObject
{
public:
	using GameObject::GameObject;

	Obstacle();

	void Update() override;

	void Pop(XMFLOAT3 pos);

	void Draw() override;

	void OnCollision();

private:
	XMFLOAT3 position;
};

