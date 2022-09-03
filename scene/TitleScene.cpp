#include "TitleScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
#include "GamePlayScene.h"

void TitleScene::Initialize(DirectXCommon* dxCommon)
{
	//スプライト共通テクスチャ読み込み
	//SpriteCommon::GetInstance()->LoadTexture(1, L"Resources/title.png");
	Sprite::LoadTexture(1, L"Resources/title.png");
	//スプライトの生成
	sprite = Sprite::Create(1, { 0.0f,0.0f });
	sprite->SetPosition({ 0,0 });

}

void TitleScene::Finalize()
{
	//スプライト個別解放
	delete sprite;
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();
	if (input->TriggerKey(DIK_SPACE))     // スペースキーが押されていたら
	{
		//シーン切り替え
		BaseScene* scene = new GamePlayScene();
		this->sceneManager->SetNextScene(scene);
	}

	//更新
	//sprite->Update();
}

void TitleScene::Draw(DirectXCommon* dxCommon)
{
	//描画前処理
	dxCommon->PreDraw();

	//スプライト共通コマンド
	Sprite::PreDraw(dxCommon->GetCmdList());
	//スプライト描画
	sprite->Draw();
	Sprite::PostDraw();
		//描画後処理
	dxCommon->PostDraw();

}