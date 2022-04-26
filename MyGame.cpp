#include "MyGame.h"
#include "GamePlayScene.h"
void MyGame::Initialize()
{
	//基底クラスの初期化
	Framework::Initialize();

#pragma region シーン初期化処理
	this->scene = new GamePlayScene();
	this->scene->Initialize(dxCommon);
}

void MyGame::Finalize()
{
	this->scene->Finalize();
	//基底クラスの終了処理
	Framework::Finalize();
}

void MyGame::Update()
{
	//基底クラスの更新処理
	Framework::Update();

	this->scene->Update();
}

void MyGame::Draw()
{
	//描画前処理
	dxCommon->PreDraw();

	//シーン描画
	this->scene->Draw();

	//デバックテキスト描画
	debugText->DrawAll();

	//描画後処理
	dxCommon->PostDraw();
}
