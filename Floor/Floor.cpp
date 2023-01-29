#include "Floor.h"

Floor::Floor() :Floor(Model::CreateFromOBJ("FloorBox"))
{
	object->SetScale({ 20.f, 5.0f, 20.f });
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
