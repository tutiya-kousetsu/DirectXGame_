#include "TitleScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
#include "GamePlayScene.h"
#include "Player.h"

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
	
	//カメラの初期化
	camera.reset(new DebugCamera(WinApp::window_width, WinApp::window_height));
	//カメラを3Dオブジェットにセット
	Object3d::SetCamera(camera.get());

	//床のオブジェクト生成
	//floorModel = Model::CreateFromOBJ("FloorBox");
	floor = TouchableObject::Create(Model::CreateFromOBJ("FloorBox"));
	floor->SetScale({ 20.f, 5.0f, 20.f });
	floor->SetPosition({ 0,-18.5f,0 });

	//データ読み込み
	skyModel = Model::CreateFromOBJ("skydome");
	skyObj = Object3d::Create();
	skyObj->SetModel(skyModel);

	//自機のオブジェクトセット+初期化
	player = player->Create(Model::CreateFromOBJ("octopus"));

	// カメラ注視点をセット
	camera->SetTarget({ 0, -30, 70 });
	camera->SetEye({ 0, 15, -30 });
	camera->SetDistance(10.0f);
}

void TitleScene::Finalize()
{
	delete player;
	delete floor;
	delete skyModel;
	delete skyObj;
	//スプライト個別解放
	delete titleSp;
	delete spaceSp;
	delete backSp;
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();
	camera->Update();
	//マウスの左クリックが押されていたら
	if (input->TriggerKey(DIK_SPACE))
	{
		spacePos = spaceSp->GetPosition();
		spacePos.y += 3.0f;
		spaceSp->SetPosition(spacePos);
		//シーン切り替え
		BaseScene* scene = new GamePlayScene();
		this->sceneManager->SetNextScene(scene);
	}
	player->Update();
	floor->Update();
	skyObj->Update();
}

void TitleScene::Draw(DirectXCommon* dxCommon)
{
	//描画前処理
	dxCommon->PreDraw();
	//スプライト描画
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCmdList());
	//背景スプライト描画

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer(dxCommon->GetCmdList());
#pragma endregion

#pragma endregion

	//3Dオブジェクト描画前処理
	Object3d::PreDraw();
	floor->Draw();
	skyObj->Draw();
	player->Draw();
	Object3d::PostDraw();

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCmdList());

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	titleSp->Draw();
	spaceSp->Draw();
	// スプライト描画後処理

	Sprite::PostDraw();

	//描画後処理
	dxCommon->PostDraw();

}