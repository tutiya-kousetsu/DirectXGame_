#include "PlayerBulletObj.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"

PlayerBulletObj* PlayerBulletObj::Create(Model* model)
{
	// �I�u�W�F�N�g�̃C���X�^���X�𐶐�
	PlayerBulletObj* instance = new PlayerBulletObj();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize(model)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool PlayerBulletObj::Initialize(Model* model)
{
	if (!Object3d::Initialize())
	{
		return false;
	}

	SetModel(model);

	// �R���C�_�[�̒ǉ�
	MeshCollider* collider = new MeshCollider;
	SetCollider(collider);
	collider->ConstructTriangles(model);

	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	return true;
}
