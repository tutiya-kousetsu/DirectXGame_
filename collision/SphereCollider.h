#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

/// <summary>
/// ���Փ˔���I�u�W�F�N�g
/// </summary>
class SphereCollider : public BaseCollider, public Sphere
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMVECTOR = DirectX::XMVECTOR;
public:
	SphereCollider(XMVECTOR offset = { 0,0,0,0 }, float radius = 1.0f) :
		offset(offset),
		radius(radius)
	{
		// ���`����Z�b�g
		shapeType = COLLISIONSHAPE_SPHERE;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	inline void SetRadius(float radius) { this->radius = radius; }

private:
	// �I�u�W�F�N�g���S����̃I�t�Z�b�g
	XMVECTOR offset;
	// ���a
	float radius;
};

