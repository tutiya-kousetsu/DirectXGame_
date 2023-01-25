#pragma once
#include "GameObject.h"
#include "Fbx_Object3d.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include <memory>

using namespace DirectX;

class Obstacle
{
public:

	Obstacle();

	void Initialize(XMFLOAT3 pos);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);

	void OnCollision();

private:
	XMFLOAT3 position;
	std::unique_ptr<Fbx_Object3d> fbxObj;

};

