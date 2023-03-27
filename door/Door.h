#pragma once
#include "GameObject.h"

using namespace DirectX;

class Door : public GameObject
{
public:
	using GameObject::GameObject;

public:
	Door();
	~Door();

	void Initialize();

	void Update();

	void Draw();

private:
	XMFLOAT3 position;
	XMFLOAT3 rotation;
};