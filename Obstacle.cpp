#include "Obstacle.h"
Obstacle::Obstacle() :Obstacle(Model::LoadFromObj("obstacle"))
{
	object->SetScale({ 2.65f, 2.65f, 2.65f });
}

void Obstacle::Update()
{
	Pop(position);
	object->SetPosition(position);
	object->Update();
}

void Obstacle::Pop(XMFLOAT3 pos)
{
	position = pos;
}

void Obstacle::Draw()
{
	if (alive) {
		object->Draw();
	}
}

void Obstacle::OnCollision()
{
}
