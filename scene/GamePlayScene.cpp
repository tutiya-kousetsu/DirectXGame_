#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"

#include "Enemy/EnemyBullet.h"

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

	player.reset(new Player());
	floor = new Floor();
	playerBullet = new PlayerBullet();
	enemyBullet = new EnemyBullet();
	obstacle = new Obstacle();
	for (auto i = 0; i < 14; i++) {
		enemy[i] = new Enemy();
		enemy[i]->Initialize();

		//敵に自機のアドレスを渡して敵が自機を使えるようにする
		enemy[i]->SetPlayer(player.get());
	}

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

	//3Dモデル解放
	//3Dオブジェクト解放
	delete postEffect[0];
	delete postEffect[1];

	//3Dオブジェクト解放
	delete skyModel;
	delete skyObj;
	delete playerBullet;
	delete enemyBullet;
	for (auto i = 0; i < 14; i++) {
		delete enemy[i];
	}
}

void GamePlayScene::Update()
{
	Input* input = Input::GetInstance();
	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();
	float dy = mouseMove.lX * scaleY;
	angleY = -dy * XM_PI;

	{
		// 追加回転分の回転行列を生成
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationY(-angleY);
		// 累積の回転行列を合成
		// ※回転行列を累積していくと、誤差でスケーリングがかかる危険がある為
		// クォータニオンを使用する方が望ましい
		matRot = matRotNew * matRot;

		// 注視点から視点へのベクトルと、上方向ベクトル
		XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

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
		vecF = XMVectorScale(vecF, 0.8f);
		// FLOAT3に変換
		XMStoreFloat3(&fTargetEye, vecF);
		XMVECTOR vecTarget = XMLoadFloat3(&target2);
		vecTarget = XMVectorScale(vecTarget, 0.8f);
		// FLOAT3に変換
		XMStoreFloat3(&target2, vecTarget);
		XMVECTOR vecEye = XMLoadFloat3(&eye);
		vecEye = XMVectorScale(vecEye, 0.8f);
		// FLOAT3に変換
		XMStoreFloat3(&eye, vecEye);
		// 大きさ計算
		length = sqrtf(pow(target2.x - eye.x, 2) + pow(target2.y - eye.y, 2) + pow(target2.z - eye.z, 2));
		fTargetEye.x = eye.x - target2.x;
		fTargetEye.y = eye.y - target2.y;
		fTargetEye.z = eye.z - target2.z;

		fTargetEye.x /= length;
		fTargetEye.y /= length;
		fTargetEye.z /= length;

		XMFLOAT3 playerRot = player->GetRotation();
		playerRot.y = atan2f(-fTargetEye.x, -fTargetEye.z);
		playerRot.y *= 180 / XM_PI;
		player->SetRotation({ 0.0f, playerRot.y, 0.0f });
	}
	//X座標,Y座標を指定して表示
	//DebugText::GetInstance()->Print("Hello,DirectX!!", 0, 0);
	//X座標,Y座標,縮尺を指定して表情
	//DebugText::GetInstance()->Print("Nihon Kogakuin", 0, 20, 2.0f);
	// マウスを表示するかどうか(TRUEで表示、FALSEで非表示)
	ShowCursor(FALSE);
	// 座標の変更を反映
	SetCursorPos(960, 540);
	player->Update();
	camera->SetFollowingTarget(player.get());
	//更新
	camera->Update();

	floor->Update();

	if (!eneFlag[0]) {
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
	}
	if (eneFlag[6] && eneFlag[7] && eneFlag[8] && eneFlag[9]) {
		enemy[10]->Update();
		enemy[11]->Update();
		enemy[12]->Update();
		enemy[13]->Update();
		enemy[14]->Update();
	}


	skyObj->Update();
	obstacle->Update();

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
	for (auto i = 0; i < 14; i++) {
		enemy[i]->Draw();
	}

	skyObj->Draw();
	floor->Draw();
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
	//判定の対象
	XMFLOAT3 posA, posB;
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullet();
	for (auto i = 0; i < 14; i++) {

		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy[i]->GetBullet();
#pragma region 自弾と敵の当たり判定
		//if (enemy[i]->GetFrameFlag()) {
		posA = enemy[i]->GetPosition();

		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			bulFlag = bullet->GetAlive();
			if (bulFlag) {
				posB = bullet->GetPosition();

				float dx = abs(posB.x - posA.x);
				float dy = abs(posB.y - posA.y);
				float dz = abs(posB.z - posA.z);

				if (dx < 1 && dy < 1 && dz < 1) {
					enemy[i]->OnCollision();
					bullet->OnCollision();
					gameScore++;
					eneFlag[i] = true;
				}
			}
		}
#pragma endregion

#pragma region 敵と自機の当たり判定
		posA = enemy[i]->GetPosition();

		posB = player->GetPosition();

		float dx = abs(posB.x - posA.x);
		float dy = abs(posB.y - posA.y);
		float dz = abs(posB.z - posA.z);

		if (dx < 1 && dy < 1 && dz < 1) {
			//enemy[i]->Hit();
			enemy[i]->OnCollision();
			player->OnCollision();
		}
#pragma endregion

#pragma region 敵弾と自機の当たり判定
		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {

			bulFlag = bullet->GetAlive();
			if (bulFlag) {
				posA = player->GetPosition();

				posB = bullet->GetPosition();

				float dx = abs(posB.x - posA.x);
				float dy = abs(posB.y - posA.y);
				float dz = abs(posB.z - posA.z);

				if (dx < 1 && dy < 1 && dz < 1) {
					bullet->OnCollision();
					player->OnCollision();
					playerLife--;
				}
			}
		}
#pragma endregion

	}

#pragma region yukaと自機の当たり判定
	//posA = floor->GetPosition();

	//posB = player->GetPosition();

	//float dx = abs(posB.x - posA.x);
	//float dy = abs(posB.y - posA.y);
	//float dz = abs(posB.z - posA.z);

	//if (dx < 1 && dy < 1 && dz < 1) {
	//	//player->FloorCollision();
	//}
#pragma endregion

	playerPos = player->GetPosition();
	player->SetPosition(playerPos);
	//プレイヤーのHPが0になったら画面切り替え
	if (playerLife <= 0 || playerPos.y <= -5) {
		//シーン切り替え
		BaseScene* scene = new GameOver();
		this->sceneManager->SetNextScene(scene);
	}
	if (eneFlag[6] && eneFlag[7] && eneFlag[8] && eneFlag[9]) {
		//シーン切り替え
		BaseScene* scene = new GameClear();
		this->sceneManager->SetNextScene(scene);
	}
}