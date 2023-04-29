#include "Obstacle.h"
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
	object->Initialize(Model::CreateFromOBJ("stone"));
	object->SetScale({ 3.4f, 3.21f, 3.4f });
	object->SetPosition(pos);
}

void Obstacle::Update()
{
	color = object->GetColor();
	color.w = 0.1f;
	//object->SetColor({ 1.0f, 1.0f, 1.0f, 0.45f });
	object->Update();
}

void Obstacle::Draw()
{
	object->Draw();
}

void Obstacle::OnCollision()
{
}