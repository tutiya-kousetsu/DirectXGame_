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

	inline void SetPosition(const DirectX::XMFLOAT3& position) { fbxObj->SetPosition(position); }
	inline const DirectX::XMFLOAT3& GetPosition() const { return fbxObj->GetPosition(); }

	inline void SetScale(const DirectX::XMFLOAT3& scale) { fbxObj->SetScale(scale); }
	inline const DirectX::XMFLOAT3& GetScale() const { return fbxObj->GetScale(); }

private:
	XMFLOAT3 position;
	std::unique_ptr<Fbx_Object3d> fbxObj;

};

