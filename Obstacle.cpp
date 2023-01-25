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

	fbxObj->SetPosition(pos);
}

void Obstacle::Update()
{
	
	fbxObj->Update();
}

void Obstacle::Draw(ID3D12GraphicsCommandList* cmdList)
{
	fbxObj->Draw(cmdList);
}

void Obstacle::OnCollision()
{
}
