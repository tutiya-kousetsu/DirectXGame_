#include "Wall.h"
#include <fstream>
#include <cassert>
Wall::Wall() :Wall(Model::CreateFromOBJ("wall"))
{
	//Initializeより前に初期化とかをやるとエラーがでてくる
	//object->SetScale({ 1.0f, 1.0f, 1.0f });
}

Wall::~Wall()
{
}

void Wall::Initialize(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation)
{
	this->position = position;
	this->rotation = rotation;
	object->SetScale({ 4.0f, 4.0f, 4.0f });
	object->SetPosition(position);
	object->SetRotation(rotation);
}

void Wall::Update()
{
	/*LoadWallPopData();
	UpdataWallPopCommand();*/
	
	object->Update();
}


void Wall::Draw()
{
	object->Draw();
}