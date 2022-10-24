#include "GameOver.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "Input.h"

void GameOver::Initialize(DirectXCommon* dxCommon)
{
	Sprite::LoadTexture(1, L"Resources/GameOver.png");
	//スプライトの生成
	sprite = Sprite::Create(1, { 0.0f,0.0f });
	sprite->SetPosition({ 0,0 });

}

void GameOver::Finalize()
{
	//スプライト個別解放
	delete sprite;
}

void GameOver::Update()
{
	Input* input = Input::GetInstance();
	if (input->TriggerKey(DIK_R))     // スペースキーが押されていたら
	{
		//シーン切り替え
		BaseScene* scene = new TitleScene();
		this->sceneManager->SetNextScene(scene);
	}
}

void GameOver::Draw(DirectXCommon* dxCommon)
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
