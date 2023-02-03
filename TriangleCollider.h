#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

class TriangleCollider : public BaseCollider, public Triangle
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMVECTOR = DirectX::XMVECTOR;
public:
	TriangleCollider(XMVECTOR offset = { 0,0,0,0 }, float radius = 1.0f) :
		offset(offset),
		radius(radius)
	{
		// ���b�V�����Z�b�g
		shapeType = COLLISIONSHAPE_MESH;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	inline const XMVECTOR& GetOffset() { return offset; }

	inline void SetOffset(const XMVECTOR& offset) { this->offset = offset; }

	inline float GetRadius() { return radius; }

	inline void SetRadius(float radius) { this->radius = radius; }

private:
	// �I�u�W�F�N�g���S����̃I�t�Z�b�g
	XMVECTOR offset;
	// ���a
	float radius;
};

