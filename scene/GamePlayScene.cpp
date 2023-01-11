#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
#include "Player.h"
#include "Enemy/EnemyBullet.h"
#include "Collision.h"
#include "ParticleManager.h"
#include "Line.h"
#include <fstream>
#include <cassert>
void GamePlayScene::Initialize(DirectXCommon* dxCommon)
{
	//スプライトの生成
	Sprite::LoadTexture(2, L"Resources/sosa_sinan.png");
	sprite = Sprite::Create(2, { 0,0 });
	sprite->SetPosition({ 0,0 });
	Sprite::LoadTexture(3, L"Resources/Life.png");
	LifeSprite = Sprite::Create(3, { 0,0 });
	Sprite::LoadTexture(4, L"Resources/Life.png");
	LifeSprite2 = Sprite::Create(4, { 26,0 });
	Sprite::LoadTexture(5, L"Resources/Life.png");
	LifeSprite3 = Sprite::Create(5, { 52,0 });

	//ポストエフェクトの初期化
	for (int i = 0; i <= 1; i++) {
		postEffect[i] = new PostEffect();
	}
	//シェーダーの挿入
	postEffect[0]->Initialize(L"Resources/shaders/PostEffectPS.hlsl");

	postEffect[1]->Initialize(L"Resources/shaders/PixelShader.hlsl");


	//音声読み込み
	//Audio::GetInstance()->SoundLoadWave("Alarm01.wav");

	//音声再生
	//audio->SoundPlayWave("Alarm01.wav", true);

	//カメラの初期化
	camera.reset(new FollowingCamera());
	//カメラを3Dオブジェットにセット
	Object3d::SetCamera(camera.get());


	//デバイスをセット
	Fbx_Object3d::SetDevice(dxCommon->GetDev());
	//カメラをセット
	Fbx_Object3d::SetCamera(camera.get());
	//グラフィックスパイプライン生成
	Fbx_Object3d::CreateGraphicsPipeline();

	//乱数の初期化
	srand((unsigned)time(NULL));

	player = new Player();
	floor = new Floor();
	playerBullet = new PlayerBullet();
	enemyBullet = new EnemyBullet();
	for (auto i = 0; i < 4; i++) {
		obstacle[i] = new Obstacle();
	}
	particleMan = new ParticleManager();
	//line = new Line();
	particleMan->Initialize(dxCommon->GetDev());
	particleMan->SetCamera(camera.get());
	enemy = new Enemy();
	enemy->Initialize();

	//敵に自機のアドレスを渡して敵が自機を使えるようにする
	enemy->SetPlayer(player);

	//データ読み込み
	skyModel = Model::LoadFromObj("skydome");
	skyObj = Object3d::Create();
	skyObj->SetModel(skyModel);
}

void GamePlayScene::Finalize()
{
	//スプライト個別解放
	delete sprite;
	delete LifeSprite;
	delete LifeSprite2;
	delete LifeSprite3;

	delete particleMan;
	//3Dモデル解放
	//3Dオブジェクト解放

	//3Dオブジェクト解放
	delete skyModel;
	delete player;
	delete skyObj;
	delete playerBullet;
	for (auto i = 0; i < 4; i++) {
		delete obstacle[i];
	}
	delete enemyBullet;
	delete enemy;
}

void GamePlayScene::Update()
{
	Input* input = Input::GetInstance();

	// マウスを表示するかどうか(TRUEで表示、FALSEで非表示)
	ShowCursor(FALSE);
	// 座標の変更を反映
	SetCursorPos(960, 540);


	camera->SetFollowingTarget(player);

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
		// FLOAT3に変換//
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
	//X座標,Y座標を指定して表示
	//DebugText::GetInstance()->Print("Hello,DirectX!!", 0, 0);
	//X座標,Y座標,縮尺を指定して表情
	//DebugText::GetInstance()->Print("Nihon Kogakuin", 0, 20, 2.0f);

	player->Update();

	particleMan->Update();
	//更新
	camera->Update();

	floor->Update();
	//line->Update();
	/*if (!eneFlag[0]) {
		enemy[0]->Update();
	}
	if (eneFlag[0]) {
		enemy[1]->Update();
		enemy[2]->Update();

	}
	if (eneFlag[1] && eneFlag[2]) {
		enemy[3]->Update();
		enemy[4]->Update();
		enemy[5]->Update();
	}
	if (eneFlag[3] && eneFlag[4] && eneFlag[5]) {
		enemy[6]->Update();
		enemy[7]->Update();
		enemy[8]->Update();
		enemy[9]->Update();
	}*/
	enemy->Update();
	skyObj->Update();
	for (auto i = 0; i < 4; i++) {
		obstaclePos[i] = obstacle[i]->GetPosition();
		obstacle[0]->SetPosition({ 13,0,13 });
		obstacle[1]->SetPosition({ -13,0,-13 });
		obstacle[2]->SetPosition({ -13,0,13 });
		obstacle[3]->SetPosition({ 13,0,-13 });

		obstacle[i]->Update();
	}
	CheckAllCollision();


}

void GamePlayScene::Draw(DirectXCommon* dxCommon)
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
	player->Draw();
	//playerBullet->Draw();
	/*for (auto i = 0; i < 14; i++) {
		enemy[i]->Draw();
	}*/
	enemy->Draw();
	for (auto i = 0; i < 4; i++) {
		obstacle[i]->Draw();
	}
	//line->Draw();
	skyObj->Draw();
	floor->Draw();
	particleMan->Draw(dxCommon->GetCmdList());
	Object3d::PostDraw();

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCmdList());

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite->Draw();
	if (playerLife >= 3) { LifeSprite3->Draw(); }
	if (playerLife >= 2) { LifeSprite2->Draw(); }
	if (playerLife >= 1) { LifeSprite->Draw(); }
	//spriteBG->Draw();
	// デバッグテキストの描画
	//debugText->DrawAll(cmdList);

	// スプライト描画後処理

	Sprite::PostDraw();

	//描画後処理
	dxCommon->PostDraw();

}

void GamePlayScene::CheckAllCollision()
{
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullet();

	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullet();
#pragma region 自弾と敵の当たり判定

	Sphere pBullet;

	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		for (auto& pb : playerBullets) {
			if (pb->GetAlive()) {
				pBullet.center = XMLoadFloat3(&pb->GetPosition());
				pBullet.radius = pb->GetScale().x;
				if (enemy->GetAlive()) {
					Sphere enemyShape;
					enemyShape.center = XMLoadFloat3(&enemy->GetPosition());
					enemyShape.radius = enemy->GetScale().z;

					if (Collision::CheckSphere2Sphere(pBullet, enemyShape)) {
						pb->OnCollision();
						enemy->OnCollision();
						eneFlag = true;
						for (int j = 0; j < 100; j++) {
							//X,Y,Z全て[-5.0f, +5.0f]でランダムに分布
							const float md_pos = 5.0f;
							XMFLOAT3 pos{};
							pos.x = enemy->GetPosition().x;
							pos.y = enemy->GetPosition().y;
							pos.z = enemy->GetPosition().z;
							//X,Y,Z全て[-0.05f, +0.05f]でランダムに分布
							const float md_vel = 0.1f;
							XMFLOAT3 vel{};
							vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
							vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
							vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
							//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
							XMFLOAT3 acc{};
							const float rnd_acc = 0.005f;
							acc.y = -(float)rand() / RAND_MAX * rnd_acc;

							//追加
							particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
						}
						posFlag = true;
					}
				}
			}
		}
	}

#pragma endregion

#pragma region 敵弾と自機の当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {

		Sphere eBullet;

		for (auto& eb : enemyBullets) {
			if (eb->GetAlive()) {
				eBullet.center = XMLoadFloat3(&eb->GetPosition());
				eBullet.radius = eb->GetScale().x;
				Sphere playerShape;
				playerShape.center = XMLoadFloat3(&player->GetPosition());
				playerShape.radius = player->GetScale().z;

				if (Collision::CheckSphere2Sphere(eBullet, playerShape)) {
					eb->OnCollision();
					//particleMan->CreateParticle(pos, 70, 4, 1.65f);
					for (int i = 0; i < 75; i++) {
						//X,Y,Z全て[-5.0f, +5.0f]でランダムに分布
						const float md_pos = 5.0f;
						XMFLOAT3 pos{};
						pos.x = player->GetPosition().x;
						pos.y = player->GetPosition().y;
						pos.z = player->GetPosition().z;
						//X,Y,Z全て[-0.05f, +0.05f]でランダムに分布
						const float md_vel = 0.1f;
						XMFLOAT3 vel{};
						vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
						vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
						vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
						//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
						XMFLOAT3 acc{};
						const float rnd_acc = 0.005f;
						acc.y = -(float)rand() / RAND_MAX * rnd_acc;

						//追加
						particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
					}
					playerLife--;
				}
			}
		}
	}

#pragma endregion

#pragma region 敵弾と障害物の当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {

		Sphere eBullet;

		for (auto& eb : enemyBullets) {
			if (eb->GetAlive()) {
				eBullet.center = XMLoadFloat3(&eb->GetPosition());
				eBullet.radius = eb->GetScale().x;
				if (enemy->GetAlive()) {
					Sphere obstacleShape;
					for (auto i = 0; i < 4; i++) {
						obstacleShape.center = XMLoadFloat3(&obstacle[i]->GetPosition());
						obstacleShape.radius = obstacle[i]->GetScale().z;


						if (Collision::CheckSphere2Sphere(eBullet, obstacleShape)) {
							eb->OnCollision();
							//obstacle->OnCollision();
						}
					}
				}
			}
		}
	}

#pragma endregion


#pragma region 自弾と障害物の当たり判定
	for (auto& pb : playerBullets) {
		if (pb->GetAlive()) {
			pBullet.center = XMLoadFloat3(&pb->GetPosition());
			pBullet.radius = pb->GetScale().x;
			Sphere obstacleShape;
			for (auto i = 0; i < 4; i++) {

				obstacleShape.center = XMLoadFloat3(&obstacle[i]->GetPosition());
				obstacleShape.radius = obstacle[i]->GetScale().z;


				if (Collision::CheckSphere2Sphere(pBullet, obstacleShape)) {
					pb->OnCollision();
				}
			}
		}
	}

#pragma endregion

#pragma region 床と自機の当たり判定
	Plane floorShape;
	floorShape.normal = XMVectorSet(0, 1, 0, 0);
	floorShape.distance = -1.0f;

	Sphere playerShape;
	playerShape.center = XMLoadFloat3(&player->GetPosition());
	playerShape.radius = player->GetScale().z;

	if (Collision::CheckSphere2Plane(playerShape, floorShape)) {
		player->FloorCollision();
	}
#pragma endregion
	XMFLOAT3 playerPos = player->GetPosition();

	//プレイヤーのHPが0になったら画面切り替え
	if (playerLife <= 0 || playerPos.y <= -5) {
		//シーン切り替え
		BaseScene* scene = new GameOver();
		this->sceneManager->SetNextScene(scene);
	}
	//if (eneFlag[6] && eneFlag[7] && eneFlag[8] && eneFlag[9]) {
	//	//シーン切り替え
	//	BaseScene* scene = new GameClear();
	//	this->sceneManager->SetNextScene(scene);
	//}
}