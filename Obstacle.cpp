#include "Obstacle.h"
Obstacle::Obstacle()
{
}

void Obstacle::Initialize(XMFLOAT3 pos)
{
	object.reset(new Object3d());
	object->Initialize();
	object->SetModel(Model::CreateFromOBJ("obstacle"));
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
