#include "GamePlayScene.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"

void GamePlayScene::Initialize()
{
	//スプライト共通テクスチャ読み込み
	SpriteCommon::GetInstance()->LoadTexture(1, L"Resources/gamePlay.png");
	//spriteCommon->LoadTexture(2, L"Resources/house.png");

	//スプライトの生成
	sprite = Sprite::Create(1, { 0,0 }, false, false);
	sprite->SetPosition({ 0,0,0 });

	//OBJからモデルデータを読み込む
	modelPost = Model::LoadFromObj("post");

	//3Dオブジェクト生成
	objPost = Object3d::Create();
	objPost->SetModel(modelPost);
	objPost2 = Object3d::Create();
	objPost2->SetModel(modelPost);
	//音声読み込み
	//Audio::GetInstance()->SoundLoadWave("Alarm01.wav");

	//音声再生
	//audio->SoundPlayWave("Alarm01.wav", true);

	//objPost->SetPosition({ -10,0,-5 });
	//objChr->SetPosition({ +10,0,+5 });
	//カメラの初期化
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//カメラを3Dオブジェットにセット
	Object3d::SetCamera(camera);

	camera->SetEye({ 0, 0, -50 });
	camera->SetTarget({ 0,0,-10 });

	//モデル名を指定してファイルを読み込む
	FbxLoader::GetInstance()->LoadModelFromFile("cube");

	//playerPos = objPost->GetPosition();
	playerPos = { -70, 0, 0 };
	//playerPos2 = objPost2->GetPosition();
	playerPos2 = { 70, 0, 0 };

}

void GamePlayScene::Finalize()
{
	delete objPost2;
	//カメラの解放
	delete camera;
	//スプライト解放
	delete sprite;
	//3Dモデル解放
	delete modelPost;
	//3Dオブジェクト解放
	delete objPost;

}

void GamePlayScene::Update()
{
	Input* input = Input::GetInstance();
	objPost->SetScale({ 1.5f, 1.5f, 1.5f });
	objPost2->SetScale({ 2.0f, 2.0f, 2.0f });
	//放物線運動
		/*if (playerPos.x <= 70) {
			playerPos.x += speed;
			playerPos.y += speed2;
			speed2 -= t;
		}
		if (playerPos.x >= 70 || playerPos.y <= -40) {
			playerPos.x = -70;
			playerPos.y = -10;
			t = 0.01;
			speed = 0.75f;
			speed2 = 0.75f;
		}*/

	if (playerPos.x <= 70) {
		playerPos.x += speed;

	}
	if (playerPos.x >= 70) {
		playerPos.x = -70;
		speed = 0.75f;
	}
	if (playerPos2.x >= -70) {
		playerPos2.x -= speed2;

	}
	if (playerPos2.x <= -70) {
		playerPos2.x = 70;
		speed = 0.75f;
	}

	//	float dx = abs(playerPos.x - playerPos2.x);

	float enemyi;
	enemyi = sqrtf((playerPos.x - playerPos2.x) * (playerPos.x - playerPos2.x) + (playerPos.y - playerPos2.y) * (playerPos.y - playerPos2.y));
	if (R + R >= enemyi) {
		speed = -speed;
		speed2 = -speed2;
	}

	objPost->SetPosition(playerPos);
	objPost2->SetPosition(playerPos2);
	//X座標,Y座標を指定して表示
	//DebugText::GetInstance()->Print("Hello,DirectX!!", 0, 0);
	//X座標,Y座標,縮尺を指定して表情
	//DebugText::GetInstance()->Print("Nihon Kogakuin", 0, 20, 2.0f);

	//更新
	objPost->Update();
	objPost2->Update();
	//objChr->Update();
	camera->Update();
	sprite->Update();
}

void GamePlayScene::Draw()
{
	//スプライト共通コマンド
	SpriteCommon::GetInstance()->PreDraw();
	//スプライト描画
	//sprite->Draw();

	//3Dオブジェクト描画前処理
	Object3d::PreDraw();

	//3Dオブジェクトの描画
	objPost->Draw();
	objPost2->Draw();
	//3Dオブジェクト描画後処理
	Object3d::PostDraw();

	//スプライト共通コマンド
	SpriteCommon::GetInstance()->PreDraw();
}