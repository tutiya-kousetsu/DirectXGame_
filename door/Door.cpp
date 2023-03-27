#include "Door.h"

Door::Door() :Door(Model::CreateFromOBJ("door"))
{
}

Door::~Door()
{
}

void Door::Initialize()
{
	object->SetScale({ 4.0f, 4.0f, 4.0f });
	position = object->GetPosition();
	//position = { 8, 8, 40 };
	object->SetPosition(position);
	object->SetRotation(rotation);
}

void Door::Update()
{
	object->Update();
}

void Door::Draw()
{
	object->Draw();
}
