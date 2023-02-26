#include "Door.h"

Door::Door() :Door(Model::CreateFromOBJ("door"))
{
}

Door::~Door()
{
}

void Door::Initialize()
{
	this->position = position;
	object->SetScale({ 4.0f, 4.0f, 4.0f });
	object->SetPosition(position);
}

void Door::Update()
{
	/*LoadWallPopData();
	UpdataWallPopCommand();*/
	object->Update();
}

void Door::Draw()
{
	object->Draw();
}
