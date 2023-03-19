#include "TitleScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
#include "GamePlayScene.h"
#include "Collision.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "MeshCollider.h"


void TitleScene::Initialize(DirectXCommon* dxCommon)
{
	//テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/Title.png");
	//スプライトの生成
	titleSp.reset(Sprite::Create(1, { 0.0f,400.0f }));

	//カメラの初期化
	camera.reset(new DebugCamera(WinApp::window_width, WinApp::window_height));
	//カメラを3Dオブジェットにセット
	Object3d::SetCamera(camera.get());

	//床のオブジェクト生成
	floor.reset(TouchableObject::Create(Model::CreateFromOBJ("FloorBox")));
	floor->SetScale({ 20.f, 5.0f, 20.f });
	floor->SetPosition({ 0,-18.5f,0 });

	//データ読み込み
	skyObj.reset(Object3d::Create());
	skyObj->SetModel(Model::CreateFromOBJ("skydome"));

	//スタート文字のオブジェクト
	startObj.reset(Object3d::Create());
	startObj->SetModel(Model::CreateFromOBJ("letter"));
	startObj->SetPosition({ 10.f, 7.5f, 30.f });
	startObj->SetRotation({ 90,0,0 });
	startObj->SetScale({ 4.0f, 4.0f, 4.0f });

	//自機のオブジェクトセット+初期化
	player.reset(Player::Create(Model::CreateFromOBJ("octopus")));

	enemy.reset(new FrontEnemy());
	enemy->Initialize({ 10, 6, 30 });
	enemy->SetPlayer(player.get());
	// カメラ注視点をセット
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();
	//プレイヤーのx軸の位置に合わせてカメラも動かす
	camera->SetTarget({ player->GetPosition().x, -20, 70 });
	camera->SetEye({ player->GetPosition().x, 10, -30 });
	camera->SetDistance(10.0f);

	camera->Update();
	enemy->Update();
	player->Update();
	floor->Update();
	skyObj->Update();
	startObj->Update();
	CheckCollision();
}

void TitleScene::CheckCollision()
{
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullet();

#pragma region 自弾と敵の当たり判定
	//敵を倒したらゲームスタート
	Sphere pBullet;
	for (auto& pb : playerBullets) {
		if (pb->GetAlive()) {
			pBullet.center = XMLoadFloat3(&pb->GetPosition());
			pBullet.radius = pb->GetScale().x;

			//前の敵
			if (enemy) {
				if (enemy->GetAlive()) {
					Sphere fEnemyShape;
					fEnemyShape.center = XMLoadFloat3(&enemy->GetPosition());
					fEnemyShape.radius = enemy->GetScale().z;

					if (Collision::CheckSphere2Sphere(pBullet, fEnemyShape)) {
						pb->OnCollision();
						enemy->OnCollision();
						if (!enemy->GetAlive()) {
							BaseScene* scene = new GamePlayScene();
							this->sceneManager->SetNextScene(scene);
						}
					}
				}
			}
		}
	}
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
	startObj->Draw();
	player->Draw();
	enemy->Draw();
	Object3d::PostDraw();

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCmdList());

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	titleSp->Draw();
	// スプライト描画後処理

	Sprite::PostDraw();

	//描画後処理
	dxCommon->PostDraw();

}