#include "MyGame.h"
#include "GamePlayScene.h"
#include "TitleScene.h"
void MyGame::Initialize()
{
	//基底クラスの初期化
	Framework::Initialize();

#pragma region シーン初期化処理
	BaseScene* scene = new TitleScene();
	//シーンマネージャーに最初のシーンをセット
	this->sceneManager->SetNextScene(scene);
}

void MyGame::Finalize()
{
	//基底クラスの終了処理
	Framework::Finalize();
}

void MyGame::Update()
{
	//基底クラスの更新処理
	Framework::Update();
}

void MyGame::Draw()
{
	//基底クラスの描画処理
	Framework::Draw();
}
