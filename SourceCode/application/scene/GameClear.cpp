#include "GameClear.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "Input.h"

void GameClear::Initialize(DirectXCommon* dxCommon)
{
	Sprite::LoadTexture(1, L"Resources/GameClear.png");
	//スプライトの生成
	sprite = Sprite::Create(1, { 0,0 });
	sprite->SetPosition({ 0,0 });

}

void GameClear::Finalize()
{
	//スプライト個別解放
	delete sprite;
}

void GameClear::Update()
{
	Input* input = Input::GetInstance();
	if (input->TriggerKey(DIK_SPACE))     // スペースキーが押されていたら
	{
		//シーン切り替え
		BaseScene* scene = new TitleScene();
		this->sceneManager->SetNextScene(scene);
	}
}

void GameClear::Draw(DirectXCommon* dxCommon)
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
