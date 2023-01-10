#include "Obstacle.h"
Obstacle::Obstacle() :Obstacle(Model::LoadFromObj("obstacle"))
{
	object->SetScale({ 2.65f, 2.65f, 2.65f });
}

void Obstacle::Update()
{
	object->Update();
}

void Obstacle::Draw()
{
	if (alive) {
		object->Draw();
	}
}
