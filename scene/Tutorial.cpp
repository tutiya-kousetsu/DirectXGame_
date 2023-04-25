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
#include "easing/Easing.h"

void Tutorial::Initialize(DirectXCommon* dxCommon)
{
	Sprite::LoadTexture(1, L"Resources/WASD_UI.png");
	wasdUI.reset(Sprite::Create(1, { 530,380,0 }));

	Sprite::LoadTexture(2, L"Resources/space_UI.png");
	spaceUI.reset(Sprite::Create(2, { 510,380,0 }));

	Sprite::LoadTexture(4, L"Resources/shot_UI.png");
	shotUI.reset(Sprite::Create(4, { 560,380,0 }));

	Sprite::LoadTexture(16, L"Resources/alignment.png");
	alignment.reset(Sprite::Create(16, { 600,200,0 }));

	//カメラの初期化
	camera.reset(new FollowingCamera());
	//カメラの初期化
	debugCam.reset(new DebugCamera(WinApp::window_width, WinApp::window_height));

	//床のオブジェクト生成
	floor.reset(TouchableObject::Create(Model::CreateFromOBJ("FloorBox")));
	floor->SetScale({ 20.f, 5.0f, 20.f });
	floor->SetPosition({ 0,-18.5f,0 });

	//データ読み込み
	skyObj.reset(Object3d::Create());
	skyObj->SetModel(Model::CreateFromOBJ("skydome"));

	//データ読み込み
	sceneMoveObj.reset(Object3d::Create());
	sceneMoveObj->SetModel(Model::CreateFromOBJ("sceneMove"));
	sceneMoveObj->SetScale({ 3,3,3 });
	sceneMoveObj->SetPosition({ 0, -2.02f, 20 });

	//データ読み込み
	arrowObj.reset(Object3d::Create());
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
	//シェーダーの挿入
	postEffect->Initialize(L"Resources/shaders/PostEffectPS.hlsl");
}

void Tutorial::Finalize()
{
}

void Tutorial::Update()
{

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

		// マウスの入力を取得
		Input::MouseMove mouseMove = input->GetMouseMove();
		float dy = mouseMove.lX * scaleY;
		angleY = -dy * XM_PI;

		{
			// 追加回転分の回転行列を生成
			XMMATRIX matRotNew = XMMatrixIdentity();
			matRotNew *= XMMatrixRotationY(-angleY);
			// 累積の回転行列を合成
			matRot = matRotNew * matRot;

			// 注視点から視点へのベクトルと、上方向ベクトル
			XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
			XMVECTOR vUp = { 0.0f, 0.5f, 0.0f, 0.0f };

			// ベクトルを回転
			vTargetEye = XMVector3Transform(vTargetEye, matRot);
			vUp = XMVector3Transform(vUp, matRot);

			// 長さ
			float length = 0.0f;

			XMFLOAT3 target1 = camera->GetTarget();
			camera->SetEye({ target1.x + vTargetEye.m128_f32[0], target1.y + vTargetEye.m128_f32[1], target1.z + vTargetEye.m128_f32[2] });
			camera->SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });

			// 注視点からずらした位置に視点座標を決定
			XMFLOAT3 target2 = camera->GetTarget();
			XMFLOAT3 eye = camera->GetEye();

			XMFLOAT3 fTargetEye = { 0.0f, 0.0f, 0.0f };
			XMVECTOR vecF = XMLoadFloat3(&fTargetEye);
			// FLOAT3に変換
			XMStoreFloat3(&fTargetEye, vecF);
			XMVECTOR vecTarget = XMLoadFloat3(&target2);
			// FLOAT3に変換
			XMStoreFloat3(&target2, vecTarget);
			XMVECTOR vecEye = XMLoadFloat3(&eye);
			// FLOAT3に変換
			XMStoreFloat3(&eye, vecEye);
			//正規化
			fTargetEye.x = eye.x - target2.x;
			fTargetEye.y = eye.y - target2.y;
			fTargetEye.z = eye.z - target2.z;

			//プレイヤーの回転
			XMFLOAT3 playerRot = player->GetRotation();
			playerRot.y = atan2f(-fTargetEye.x, -fTargetEye.z);
			playerRot.y *= 180 / XM_PI;
			player->SetRotation({ 0.0f, playerRot.y, 0.0f });
		}
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
				player->ScaleChange();
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
	if (player->GetPosition().y <= -5) {
		player->SetPosition({ 0,10,0 });
	}

	//各オブジェクトの更新
	postEffect->SetRadius(startEfRadius);
	camera->Update();
	debugCam->Update();
	floor->Update();
	skyObj->Update();
	enemy->TitleUpdate();
	if (zonePop) {
		sceneMoveObj->Update();
	}
	arrowObj->Update();
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

	floor->Draw();
	skyObj->Draw();
	player->TutorialDraw(rotateFlag);
	enemy->Draw();

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
