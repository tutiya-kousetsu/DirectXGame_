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

	void Draw();

	void OnCollision();

	inline void SetPosition(const DirectX::XMFLOAT3& position) { object->SetPosition(position); }
	inline const DirectX::XMFLOAT3& GetPosition() const { return object->GetPosition(); }

	inline void SetScale(const DirectX::XMFLOAT3& scale) { object->SetScale(scale); }
	inline const DirectX::XMFLOAT3& GetScale() const { return object->GetScale(); }

private:
	XMFLOAT3 position;
	std::unique_ptr<Object3d> object;

};

