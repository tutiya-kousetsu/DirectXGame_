#pragma once
#include "Object3d.h"

class Door
{
public:
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:
	Door();
	~Door();

	void Initialize();

	void Update();

	void DoorMove(int phaseCount);

	void Draw();

private:
	std::unique_ptr<Object3d> object[8];
	XMFLOAT3 position[8];
	XMFLOAT3 rotation[8];
	//イーズアウト用frame
	float outFrame[8]{};
};