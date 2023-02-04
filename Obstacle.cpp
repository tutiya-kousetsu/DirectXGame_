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
	object->SetScale({ 3.01f, 3.01f, 3.01f });
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