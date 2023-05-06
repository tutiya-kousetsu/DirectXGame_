#include "Obstacle.h"
#include "BaseCollider.h"
#include "CollisionManager.h"
#include <fstream>

Obstacle::~Obstacle()
{
	if (collider) {
		//コリジョンマネージャから登録を解除
		CollisionManager::GetInstance()->RemoveCollider(collider);
		delete collider;
	}
}

bool Obstacle::Initialize(XMFLOAT3 position)
{
	object.reset(new TouchableObject());
	object->Initialize(Model::CreateFromOBJ("stone"));
	object->SetScale({ 3.4f, 3.21f, 3.4f });
	object->SetPosition(position);
	return true;

}

void Obstacle::Update()
{
	object->Update();
}

void Obstacle::Draw()
{
	object->Draw();
}

void Obstacle::OnCollision(bool flag)
{

	if (flag) {
		color = object->GetColor();
		color.w = 0.45f;
		object->SetColor({ 1.0f, 1.0f, 1.0f, color.w });
	}
	if (!flag) {
		object->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	}
}