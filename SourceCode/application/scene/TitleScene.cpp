#include "TitleScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
#include "GamePlayScene.h"
#include "Tutorial.h"
#include "Collision.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "MeshCollider.h"


void TitleScene::Initialize(DirectXCommon* dxCommon)
{
	//テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/Title.png");
	//スプライトの生成
	titleSp.reset(Sprite::Create(1, { 0,0 }));

	//テクスチャ読み込み
	Sprite::LoadTexture(2, L"Resources/space.png");
	//スプライトの生成
	spaceSp.reset(Sprite::Create(2, { 0,0 }));

	//カメラの初期化
	camera.reset(new DebugCamera(WinApp::window_width, WinApp::window_height));
	//カメラを3Dオブジェットにセット
	Object3d::SetCamera(camera.get());

	//床のオブジェクト生成
	floor.reset(TouchableObject::Create(Model::CreateFromOBJ("FloorBox")));
	floor->SetScale({ 20.f, 5.0f, 20.f });
 	floor->SetPosition({ 0,-18.5f,0 });

	//データ読み込み
	skyObj = Object3d::Create();
	skyObj->SetModel(Model::CreateFromOBJ("skydome"));

	//自機のオブジェクトセット+初期化
	player.reset(Player::Create(Model::CreateFromOBJ("octopus")));
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();
	//フラグがfalseの時カメラをplayer中心にして回す
	angle += XMConvertToRadians(0.6f);
	cameraPos = camera->GetEye();
	cameraPos.x = 50 * sinf(angle);
	cameraPos.y = 30;
	cameraPos.z = 50 * cosf(angle);
	//プレイヤーのx軸の位置に合わせてカメラも動かす
	camera->SetTarget(player->GetPosition());
	camera->SetEye(cameraPos);
	camera->SetDistance(10.0f);
	if (input->TriggerKey(DIK_SPACE))//スペースキーが押されていたら
	{
		//シーン切り替え
		BaseScene* scene = new Tutorial();
		this->sceneManager->SetNextScene(scene);
	}

	camera->Update();
	player->StopUpdate();
	player->SetRotation({ 0,180,0 });
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