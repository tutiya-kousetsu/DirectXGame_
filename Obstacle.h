#pragma once
#include "GameObject.h"
#include "Fbx_Object3d.h"
#include "DirectXCommon.h"
#include "Object3d.h"
#include "CollisionInfo.h"
#include <DirectXMath.h>
#include <memory>

using namespace DirectX;

class Obstacle : Object3d
{
public:

	Obstacle();

	void Initialize(DirectX::XMFLOAT3 pos);

	void Update();

	void Draw();

	void OnCollision();

	/// <summary>
	/// �R���C�_�[�̃Z�b�g
	/// </summary>
	/// <param name="collider">�R���C�_�[</param>
	void SetCollider(BaseCollider* collider);

private:
	DirectX::XMFLOAT3 position;
	std::unique_ptr<Object3d> object;

};

