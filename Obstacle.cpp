#include "Obstacle.h"
Obstacle::Obstacle()
{
}

void Obstacle::Initialize(XMFLOAT3 pos)
{
	fbxObj.reset(new Fbx_Object3d());
	fbxObj->Initialize();
	fbxObj->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("cube"));
	fbxObj->SetScale({ 0.2f, 0.2f, 0.2f });

	this->position = pos;
}

void Obstacle::Update()
{
	/*Pop(position);*/
	fbxObj->SetPosition(position);
	fbxObj->Update();
}

void Obstacle::Draw(ID3D12GraphicsCommandList* cmdList)
{
	fbxObj->Draw(cmdList);
}

void Obstacle::OnCollision()
{
}
