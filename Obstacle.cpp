#include "Obstacle.h"
#include "SphereCollider.h"
#include "BaseCollider.h"
#include "CollisionManager.h"


Obstacle::~Obstacle()
{
	if (collider) {
		//コリジョンマネージャから登録を解除
		CollisionManager::GetInstance()->RemoveCollider(collider);
		delete collider;
	}
}

void Obstacle::Initialize(DirectX::XMFLOAT3 pos)
{
	object.reset(new TouchableObject());
	object->Initialize(Model::CreateFromOBJ("obstacle"));
	object->SetScale({ 3.f, 3.f, 3.f });
	object->SetPosition(pos);
}

void Obstacle::Update()
{
	
	object->Update();
}

void Obstacle::Draw()
{
	object->Draw();
}

void Obstacle::OnCollision()
{
}

//void Obstacle::SetCollider(BaseCollider* collider)
//{
//	collider->SetObject(this);
//	this->collider = collider;
//	//コリジョンマネージャに登録
//	CollisionManager::GetInstance()->AddCollider(collider);
//	UpdateWorldMatrix();
//	//コライダーを更新しておく
//	collider->Update();
//}
