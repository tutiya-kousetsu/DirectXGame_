#pragma once
#include "GameObject.h"
#include "Object3d.h"
#include <sstream>

class Wall : public GameObject
{
public:
	using GameObject::GameObject;

public:
	Wall();

	~Wall();

	void Initialize(DirectX::XMFLOAT3 position);

	void Update();

	void Draw();

	void LoadWallPopData();

	void UpdataWallPopCommand();

private:
	DirectX::XMFLOAT3 position;
	//ï«î≠ê∂ÇÃÉRÉ}ÉìÉh
	std::stringstream wallPopCom;
};

