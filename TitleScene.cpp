#include "TitleScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
#include "GamePlayScene.h"
TitleScene::TitleScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{
}

void TitleScene::Initialize(DirectXCommon* dxCommon)
{
	//スプライト共通テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/title.png");

	//スプライトの生成
	sprite = Sprite::Create(1, { 0.0f,0.0f });
	sprite->SetPosition({ 0,0 });

	//カメラの初期化
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//カメラを3Dオブジェットにセット
	Object3d::SetCamera(camera);

	//camera->SetEye({ 0, 0, -50 });
	camera->SetTarget({ 0,0,0 });
	camera->SetDistance(100.0f);
}

void TitleScene::Finalize()
{
	delete camera;
	//スプライト個別解放
	delete sprite;
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();
	if (input->TriggerKey(DIK_SPACE))     // スペースキーが押されていたら
	{
		//シーン切り替え
		BaseScene* scene = new GamePlayScene(this->sceneManager);
		this->sceneManager->SetNextScene(scene);
	}

	// 座標操作
	/*if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{

	}*/

	//更新
	camera->Update();
	//sprite->Update();
}

void TitleScene::Draw(DirectXCommon* dxCommon)
{
	//スプライト共通コマンド
	Sprite::PreDraw(dxCommon->GetCmdList());
	//スプライト描画
	sprite->Draw();
}