#pragma once
#include "GameObject.h"
#include <DirectXMath.h>
using namespace DirectX;

class Line : public GameObject
{
public:
	using GameObject::GameObject;

	Line();

	void Initialize(XMFLOAT3 pos);

	void Update() override;

	void Draw() override;

private:
	XMFLOAT3 position;
};

