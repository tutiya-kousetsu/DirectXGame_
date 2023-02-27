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
	position = { 8, 8, 40 };
	object->SetPosition(position);
}

void Door::Update()
{
	/*LoadWallPopData();
	UpdataWallPopCommand();*/
	MoveSlide();
	object->Update();
}

void Door::MoveSlide()
{
	position = object->GetPosition();
	if (position.x >= 0) {
		position.x -= 0.05;
	}
	object->SetPosition(position);
}

void Door::Draw()
{
	object->Draw();
}
