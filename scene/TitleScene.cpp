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
	//テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/Title.png");
	//スプライトの生成
	titleSp = Sprite::Create(1, { 0.0f,0.0f });
	titleSp->SetPosition({ 0,0 });
	//テクスチャ読み込み
	Sprite::LoadTexture(2, L"Resources/space.png");
	spaceSp = Sprite::Create(2, { 0.0f,0.0f });
	spaceSp->SetPosition({ 0,0 });

}

void TitleScene::Finalize()
{
	//スプライト個別解放
	delete titleSp;
	delete spaceSp;
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();
	//マウスの左クリックが押されていたら
	if (input->TriggerKey(DIK_SPACE))
	{
		//シーン切り替え
		BaseScene* scene = new GamePlayScene();
		this->sceneManager->SetNextScene(scene);
	}
}

void TitleScene::Draw(DirectXCommon* dxCommon)
{
	//描画前処理
	dxCommon->PreDraw();

	//スプライト共通コマンド
	Sprite::PreDraw(dxCommon->GetCmdList());
	//スプライト描画
	titleSp->Draw();
	spaceSp->Draw();
	Sprite::PostDraw();
		//描画後処理
	dxCommon->PostDraw();

}