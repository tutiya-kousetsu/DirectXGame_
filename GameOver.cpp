#include "GameOver.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "Input.h"

void GameOver::Initialize(DirectXCommon* dxCommon)
{
	//スプライト共通テクスチャ読み込み
	SpriteCommon::GetInstance()->LoadTexture(1, L"Resources/GameOver.png");

	//スプライトの生成
	sprite = Sprite::Create(1, { 0,0 }, false, false);
	sprite->SetPosition({ 0,0,0 });

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
		SceneManager::GetInstance()->SetNextScene(scene);
	}

	//更新
	sprite->Update();
}

void GameOver::Draw(DirectXCommon* dxCommon)
{
	//スプライト共通コマンド
	SpriteCommon::GetInstance()->PreDraw();
	//スプライト描画
	sprite->Draw();
}
