#include "Obstacle.h"
Obstacle::Obstacle() :Obstacle(Model::LoadFromObj("obstacle"))
{
	object->SetScale({ 2.56f, 2.56f, 2.56f });
}

Obstacle::~Obstacle()
{
	
}

void Obstacle::Initialize(XMFLOAT3 pos)
{
	this->position = pos;
}

void Obstacle::Update()
{
	object->SetPosition(position);
	object->Update();
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
