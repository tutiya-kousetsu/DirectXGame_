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
	//カメラの初期化
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//カメラを3Dオブジェットにセット
	Object3d::SetCamera(camera);

	camera->SetEye({ 0, 0, -50 });
	camera->SetTarget({ 0,0,-10 });

	//モデル名を指定してファイルを読み込む
	FbxLoader::GetInstance()->LoadModelFromFile("cube");

	//プレイヤーの初期位置
	playerPos = { 0, 0, 0 };
	//playerPos2 = { -15, 0, 0 };

	// 全ての球の初期値
	playerPos2 = { 0, 15, 0 };
	m = 1.f;
	v = { 0, 0, 0 };
	a = { 0, -g, 0 };
	r = 1.0f;


}

void GamePlayScene::Finalize()
{
	//3Dオブジェクト解放
	delete objPost;
	delete objPost2;
	//3Dモデル解放
	delete modelPost;
	//カメラの解放
	delete camera;
	//スプライト解放
	delete sprite;
}

void GamePlayScene::Update()
{
	Input* input = Input::GetInstance();
	objPost->SetScale({ 1.5f, 1.5f, 1.5f });
	objPost2->SetScale({ 3.0f, 3.0f, 3.0f });

	/*自機の動き*/
	r = Angle * 3.14f / 180.0f;	//度数法の角度を弧度法に変換

	float addX = cos(r) * Length;		//三角関数を使用し、円の位置を割り出す。
	float addY = sin(r) * Length;

	playerPos.x = centerX + addX;						//結果で出てきた位置を中心位置に加算し、それを描画位置とする
	playerPos.y = centerY + addY;
	Angle += 3.0f;							//角度更新、時計回り
	//// 値を更新(反発)
	//v.y += a.y;
	//playerPos2.y += v.y;

	//v.z += a.z;
	//playerPos2.z += v.z;

	//v.x += a.x;
	//playerPos2.x += v.x;

	//// -10以下に移動したら跳ね返る
	//if (playerPos2.y < -10) {
	//	playerPos2.y -= v.y;
	//	v.y *= -repulCoe;
	//}
	objPost->SetPosition(playerPos);
	objPost2->SetPosition(playerPos2);

	//X座標,Y座標を指定して表示
	//DebugText::GetInstance()->Print("Hello,DirectX!!", 0, 0);
	//X座標,Y座標,縮尺を指定して表情
	//DebugText::GetInstance()->Print("Nihon Kogakuin", 0, 20, 2.0f);

	//更新
	objPost->Update();
	//objPost2->Update();
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