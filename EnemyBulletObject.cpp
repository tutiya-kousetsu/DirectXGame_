#include "EnemyBulletObject.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"

EnemyBulletObject* EnemyBulletObject::Create(Model* model)
{
	// �I�u�W�F�N�g�̃C���X�^���X�𐶐�
	EnemyBulletObject* instance = new EnemyBulletObject();
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

bool EnemyBulletObject::Initialize(Model* model)
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

	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
	return true;
}
