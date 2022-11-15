#include "Obstacle.h"

Obstacle::Obstacle() :Obstacle(Model::LoadFromObj("obstacle"))
{
	object->SetScale({ 1.65f, 1.65f, 1.65f });
}

void Obstacle::Updata()
{
	position = object->GetPosition();
	position = { 0,0,0 };
	object->SetPosition(position);
	object->Updata();
}

void Obstacle::Draw()
{
	object->Draw();
}
