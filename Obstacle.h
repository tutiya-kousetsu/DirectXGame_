#pragma once
#include "GameObject.h"
#include <DirectXMath.h>
using namespace DirectX;

class Obstacle : public GameObject
{
public:
	using GameObject::GameObject;

	Obstacle();

	~Obstacle();

	void Initialize(XMFLOAT3 pos);

	void Update() override;

	void Draw() override;

	void OnCollision();

private:
	XMFLOAT3 position;
};

