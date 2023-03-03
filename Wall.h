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
	//壁発生のコマンド
	std::stringstream wallPopCom;
};

////左の壁
//class LeftWall : public GameObject
//{
//public:
//	using GameObject::GameObject;
//
//public:
//	LeftWall();
//
//	~LeftWall();
//
//	void LeftInitialize(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation);
//
//	void LeftUpdate();
//
//	void LeftDraw();
//
//private:
//	DirectX::XMFLOAT3 position;
//
//	//壁発生のコマンド
//	std::stringstream wallPopCom;
//};