#include "GamePlayScene.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"

void GamePlayScene::Initialize(DirectXCommon* dxCommon)
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

	//音声読み込み
	//Audio::GetInstance()->SoundLoadWave("Alarm01.wav");

	//音声再生
	//audio->SoundPlayWave("Alarm01.wav", true);

	
	//objChr->SetPosition({ +10,0,+5 });
	//カメラの初期化
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//カメラを3Dオブジェットにセット
	Object3d::SetCamera(camera);

	//camera->SetEye({ 0, 0, -50 });
	camera->SetTarget({ 0,20,0 });
	camera->SetDistance(100.0f);
	//モデル名を指定してファイルを読み込む
	//FbxLoader::GetInstance()->LoadModelFromFile("cube");
	fbxmodel1 = FbxLoader::GetInstance()->LoadModelFromFile("cube");

	//デバイスをセット
	Fbx_Object3d::SetDevice(dxCommon->GetDev());
	//カメラをセット
	Fbx_Object3d::SetCamera(camera);
	//グラフィックスパイプライン生成
	Fbx_Object3d::CreateGraphicsPipeline();

	//3Dオブジェクト生成とモデルのセット
	fbxobject1 = new Fbx_Object3d;
	fbxobject1->Initialize();
	fbxobject1->SetModel(fbxmodel1);
}

void GamePlayScene::Finalize()
{
	delete fbxobject1;
	delete fbxmodel1;
	delete camera;
	//スプライト個別解放
	delete sprite;
	//3Dモデル解放
	delete modelPost;
	//3Dオブジェクト解放
	delete objPost;
}

void GamePlayScene::Update()
{
	Input* input = Input::GetInstance();
	objPost->SetPosition({ 0, 15,10 });
	objPost->SetRotation({ -45, 0, 0 });
	//if (input->PushKey(DIK_SPACE))     // スペースキーが押されていたら
	//{

	//}

	// 座標操作
	/*if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{

	}*/

	//X座標,Y座標を指定して表示
	DebugText::GetInstance()->Print("Hello,DirectX!!", 0, 0);
	//X座標,Y座標,縮尺を指定して表情
	DebugText::GetInstance()->Print("Nihon Kogakuin", 0, 20, 2.0f);

	//更新
	objPost->Update();
	//objChr->Update();
	camera->Update();
	fbxobject1->Update();
	sprite->Update();
	
}

void GamePlayScene::Draw()
{
	// コマンドリストの取得
	//ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	//スプライト共通コマンド
	SpriteCommon::GetInstance()->PreDraw();
	//スプライト描画
	sprite->Draw();

	//3Dオブジェクト描画前処理
	Object3d::PreDraw();
	
	//3Dオブジェクトの描画
	//objPost->Draw();
	
	fbxobject1->Draw(cmdList);
	//3Dオブジェクト描画後処理
	Object3d::PostDraw();

	//スプライト共通コマンド
	SpriteCommon::GetInstance()->PreDraw();
}