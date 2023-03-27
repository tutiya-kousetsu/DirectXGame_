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

	void Initialize(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation);

	void Update();

	void Draw();

private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	//ï«î≠ê∂ÇÃÉRÉ}ÉìÉh
	std::stringstream wallPopCom;
};
