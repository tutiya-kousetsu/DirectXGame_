#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"

void GamePlayScene::Initialize(DirectXCommon* dxCommon)
{
	if (!Sprite::LoadTexture(1, L"Resources/gamePlay.png")) {
		assert(0);
		return;
	}
	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	//スプライトの生成
	//sprite = Sprite::Create(1, { 0,0 }, false, false);
	//sprite->SetPosition({ 0,0 });

	//スプライトの生成
	//sprite1 = Sprite::Create(2, { 0,0 }, false, false);
	// テクスチャ2番に読み込み
	Sprite::LoadTexture(2, L"Resources/tex1.png");
	sprite = Sprite::Create(2, { 0.0f,0.0f });

	//sprite1->SetPosition({ 0,0 });

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
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//カメラを3Dオブジェットにセット
	Object3d::SetCamera(camera);

	camera->SetEye({ 0, 10, -30 });
	camera->SetTarget({ 0,0,30 });
	camera->SetDistance(0.0f);

	//デバイスをセット
	Fbx_Object3d::SetDevice(dxCommon->GetDev());
	//カメラをセット
	Fbx_Object3d::SetCamera(camera);
	//グラフィックスパイプライン生成
	Fbx_Object3d::CreateGraphicsPipeline();

	//乱数の初期化
	srand((unsigned)time(NULL));

	player = new Player();
	playerBullet = new PlayerBullet();
	for (int i = 0; i < 4; i++) {
		enemy[i] = new Enemy();
		enemy[i]->Initialize();
	}
		player->Initialize(input);
		playerBullet->Initialize(input, player);

	//データ読み込み
	groundModel = Model::LoadFromObj("ground");
	groundObj = Object3d::Create();
	groundObj->SetModel(groundModel);

	//データ読み込み
	skyModel = Model::LoadFromObj("skydome");
	skyObj = Object3d::Create();
	skyObj->SetModel(skyModel);

}

void GamePlayScene::Finalize()
{

	delete camera;
	//スプライト個別解放
	delete sprite1;
	delete sprite;
	//3Dモデル解放
	//3Dオブジェクト解放
	delete postEffect[0];
	delete postEffect[1];

	//3Dオブジェクト解放
	delete skyModel;
	delete skyObj;
	delete groundModel;
	delete groundObj;
	delete player;
	delete playerBullet;
	for (int i = 0; i < 4; i++) {
		delete enemy[i];
	}
}

void GamePlayScene::Update()
{
	Input* input = Input::GetInstance();

	//X座標,Y座標を指定して表示
	//DebugText::GetInstance()->Print("Hello,DirectX!!", 0, 0);
	//X座標,Y座標,縮尺を指定して表情
	//DebugText::GetInstance()->Print("Nihon Kogakuin", 0, 20, 2.0f);

	//更新
	camera->Update();
	camera->Update();
	player->Update();
	playerBullet->Update();
	for (int i = 0; i < 4; i++) {
		enemy[i]->Update();
	}
	groundObj->Update();
	skyObj->Update();
	Collision();
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
	spriteBG->Draw();
	//sprite1->Draw();

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
	playerBullet->Draw();
	for (int i = 0; i < 4; i++) {
		enemy[i]->Draw();
	}
	skyObj->Draw();
	groundObj->Draw();
	Object3d::PostDraw();

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCmdList());

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	//sprite->Draw();
	//spriteBG->Draw();
	// デバッグテキストの描画
	//debugText->DrawAll(cmdList);

	// スプライト描画後処理
	
	Sprite::PostDraw();

	//描画後処理
	dxCommon->PostDraw();

}

void GamePlayScene::Collision()
{
	for (int i = 0; i < 4; i++) {
		//プレイヤーと敵の衝突判定
		//敵が存在すれば
		if (enemy[i]->GetFlag()) {
			//座標
			XMFLOAT3 playerPosition = player->GetPosition();
			XMFLOAT3 enemyPosition = enemy[i]->GetPosition();

			//差を求める
			float dx = abs(playerPosition.x - enemyPosition.x);
			float dy = abs(playerPosition.y - enemyPosition.y);
			float dz = abs(playerPosition.z - enemyPosition.z);

			if (enemyPosition.z <= -5) {
				playerLife--;
				enemy[i]->Hit();
			}
		}
		//弾と敵の当たり判定
		//敵が存在すれば
		if (enemy[i]->GetFrameFlag()) {
			if (enemy[i]->GetFlag()) {
				//座標
				XMFLOAT3 shootPosition = playerBullet->GetPosition();
				XMFLOAT3 enemyPosition = enemy[i]->GetPosition();

				//差を求める
				float dx = abs(enemyPosition.x - shootPosition.x);
				float dy = abs(enemyPosition.y - shootPosition.y);
				float dz = abs(enemyPosition.z - shootPosition.z);

				if (dx < 1 && dy < 1 && dz < 1) {
					gameScore++;
					enemy[i]->frameFlag = 0;
					enemy[i]->Hit();
					playerBullet->Hit();
				}
			}
		}
	}

	//プレイヤーのHPが0になったら画面切り替え
	if (playerLife == 0) {
		//シーン切り替え
		BaseScene* scene = new GameOver();
		this->sceneManager->SetNextScene(scene);
	}
	if (gameScore == 5) {
		//シーン切り替え
		BaseScene* scene = new GameClear();
		this->sceneManager->SetNextScene(scene);
	}
}
