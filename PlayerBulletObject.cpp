#include "PlayerBulletObject.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"

PlayerBulletObject* PlayerBulletObject::Create(Model* model)
{
	// オブジェクトのインスタンスを生成
	PlayerBulletObject* instance = new PlayerBulletObject();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(model)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool PlayerBulletObject::Initialize(Model* model)
{
	if (!Object3d::Initialize())
	{
		return false;
	}

	SetModel(model);

	// コライダーの追加
	MeshCollider* collider = new MeshCollider;
	SetCollider(collider);
	collider->ConstructTriangles(model);

	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	return true;
}
