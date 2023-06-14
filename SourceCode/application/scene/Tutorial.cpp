#include "Tutorial.h"
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
#include "ParticleManager.h"
#include "Easing.h"

void Tutorial::Initialize(DirectXCommon* dxCommon)
{
	Audio* audio = Audio::GetInstance();

	Sprite::LoadTexture(1, L"Resources/WASD_UI.png");
	wasdUI.reset(Sprite::Create(1, { 635,420 }));

	Sprite::LoadTexture(2, L"Resources/space_UI.png");
	spaceUI.reset(Sprite::Create(2, { 640,420 }));

	Sprite::LoadTexture(4, L"Resources/shot_UI.png");
	shotUI.reset(Sprite::Create(4, { 640,420 }));

	Sprite::LoadTexture(16, L"Resources/alignment.png");
	alignment.reset(Sprite::Create(16, { 640,240 }));

	//カメラの初期化
	camera.reset(new FollowingCamera());
	//カメラの初期化
	debugCam.reset(new DebugCamera(WinApp::window_width, WinApp::window_height));

	//床のオブジェクト生成
	floor.reset(TouchableObject::Create(Model::CreateFromOBJ("FloorBox")));
	floor->SetScale({ 20.f, 5.0f, 20.f });
	floor->SetPosition({ 0,-18.5f,0 });

	//データ読み込み
	skyObj = Object3d::Create();
	skyObj->SetModel(Model::CreateFromOBJ("skydome"));

	//データ読み込み
	sceneMoveObj = Object3d::Create();
	sceneMoveObj->SetModel(Model::CreateFromOBJ("sceneMove"));
	sceneMoveObj->SetScale({ 3,3,3 });
	sceneMoveObj->SetPosition({ 0.f, -2.02f, 20.f });

	//データ読み込み
	arrowObj = Object3d::Create();
	arrowObj->SetModel(Model::CreateFromOBJ("arrow"));
	arrowObj->SetScale({ 1.5f,1.5f,1.5f });
	arrowObj->SetPosition({ 0, 2.02f, 20 });

	//自機のオブジェクトセット+初期化
	player.reset(Player::Create(Model::CreateFromOBJ("octopus")));
	player->SetPosition({ 0, -1.8f, 0 });
	enemy.reset(new FrontEnemy());
	enemy->Initialize({ 0, 5, 50 });

	//パーティクのインスタンス
	particleMan = ParticleManager::GetInstance();
	particleMan->Initialize();
	particleMan->SetCamera(camera.get());

	postEffect.reset(new PostEffect());
	postEffect->CreateGraphicsPipelineState(L"Resources/shaders/PostEffectPS.hlsl");
	//シェーダーの挿入
	postEffect->Initialize();
}

void Tutorial::Finalize()
{
}

void Tutorial::Update()
{
	audio = Audio::GetInstance();
	audio->SoundPlayWave("water.wav", true);
	if (!startFlag) {
		//中心から明るくする
		startEfRadius += 10.5f;
		if (startEfRadius >= 1000) {
			startFlag = true;
			startEfRadius -= 10.5f;
		}
		
	}
	Input* input = Input::GetInstance();

	// マウスを表示するかどうか(TRUEで表示、FALSEで非表示)
	ShowCursor(FALSE);
	// 座標の変更を反映
	SetCursorPos(960, 540);
	if (!rotateFlag) {

		//カメラを3Dオブジェットにセット
		nowCamera = camera.get();
		//カメラを3Dオブジェットにセット
		Object3d::SetCamera(nowCamera);

		camera->SetFollowingTarget(player.get());
		player->Mouse();
		player->TutorialUpdate();
	}
	if (rotateFlag) {
		rotateTime--;
		//追従カメラから普通のカメラに変更
		nowCamera = debugCam.get();
		Object3d::SetCamera(nowCamera);
		nowCamera->SetEye({ player->GetPosition().x, 2.0f, 0 });
		nowCamera->SetTarget(player->GetPosition());
		//回転させる
		playerRot = player->GetRotation();
		playerRot.y += 3.0f;
		player->SetRotation(playerRot);
		if (rotateTime <= 0) {
			//タイムが0になったら自機を上にあげる
			playerPos = player->GetPosition();
			if (easFrame < 1.0f) {
				easFrame += 0.01f;
				//自機の大きさを小さくさせる関数
				player->ScaleSmall();
			}

			playerPos.y = Ease(In, Cubic, easFrame, playerPos.y, 40.0f);
			playerScale = player->GetScale();
			//プレイヤーのスケールが0になったらシーン移動
			if (playerScale.x <= 0.f && playerScale.y <= 0.f &&playerScale.z <= 0.f) {
				BaseScene* scene = new GamePlayScene();
				this->sceneManager->SetNextScene(scene);
			}
			player->SetPosition(playerPos);
		}
		player->StopUpdate();
	}


	//プレイヤーがステージから落ちたらステージの真ん中に戻す
	if (player->GetPosition().y <= -20) {
		player->SetPosition({ 0,10,0 });
	}

	//各オブジェクトの更新
	postEffect->SetRadius(startEfRadius);
	skyObj->Update();
	enemy->TitleUpdate();

	floor->Update();
	if (zonePop) {
		sceneMoveObj->Update();
	}
	arrowObj->Update();
	camera->Update();
	debugCam->Update();

	//当たり判定
	CheckAllCollision();
	particleMan->Update();
}

void Tutorial::Draw(DirectXCommon* dxCommon)
{
	//描画前処理
	//dxCommon->PreDraw();
	//スプライト描画
#pragma region 背景スプライト描画
	postEffect->PreDrawScene(dxCommon->GetCmdList());

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
	skyObj->Draw();
	enemy->Draw();

	floor->Draw();
	player->TutorialDraw(rotateFlag);

	if (zonePop) {
		sceneMoveObj->Draw();
		arrowObj->Draw();
	}
	if (!rotateFlag) {
		particleMan->Draw();
	}
	Object3d::PostDraw();

	postEffect->PostDrawScene(dxCommon->GetCmdList());

	//描画前処理
	dxCommon->PreDraw();
	postEffect->Draw(dxCommon->GetCmdList());

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCmdList());

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	if (player->GetPhase() == 0) {
		wasdUI->Draw();
	}
	if (player->GetPhase() == 1) {
		spaceUI->Draw();
	}
	if (player->GetPhase() == 2) {
		shotUI->Draw();
	}
	if (!rotateFlag) {
		alignment->Draw();
	}
	
	// スプライト描画後処理
	Sprite::PostDraw();

	//描画後処理
	dxCommon->PostDraw();
}

void Tutorial::CheckAllCollision()
{
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullet();

#pragma region 自弾と敵の当たり判定

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
							zonePop = true;
						}
					}
				}
			}
		}
#pragma endregion
	}

#pragma region 自機とワープゾーンの当たり判定
	Sphere pShape;
	if (player && zonePop) {
		if (player->GetAlive()) {
			pShape.center = XMLoadFloat3(&player->GetPosition());
			pShape.radius = player->GetScale().x;
			
			Sphere zoneShape;
			if (sceneMoveObj) {
				zoneShape.center = XMLoadFloat3(&sceneMoveObj->GetPosition());
				zoneShape.radius = sceneMoveObj->GetScale().x - 2.0f;
				zoneShape.radius = sceneMoveObj->GetScale().y - 2.5f;
				if (Collision::CheckSphere2Sphere(pShape, zoneShape)) {
					rotateFlag = true;
				}
			}
		}
	}
#pragma endregion

}
