#include "Floor.h"

Floor::Floor() :Floor(Model::LoadFromObj("FloorBox"))
{
	object->SetScale({ 15.0f, 5.0f, 15.0f });
}

void Floor::Update()
{
	position = object->GetPosition();
	position = {0,-18.5f,0};
	object->SetPosition(position);
	object->Update();
}

void Floor::Draw()
{
	object->Draw();
}
