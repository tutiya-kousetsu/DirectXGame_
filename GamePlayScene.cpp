#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"

GamePlayScene::GamePlayScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{
}

void GamePlayScene::Initialize(DirectXCommon* dxCommon)
{
	//スプライト共通テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/gamePlay.png");
	//SpriteCommon::GetInstance()->LoadTexture(1, L"Resources/gamePlay.png");
	//スプライトの生成
	sprite = Sprite::Create(1, { 0.0f,0.0f });

	Sprite::LoadTexture(2, L"Resources/tex1.png");

	Sprite::LoadTexture(100, L"Resources/white1x1.png");
	postEffect = new PostEffect();
	postEffect->Initialize();


	FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());

	//OBJからモデルデータを読み込む
	modelPost = Model::LoadFromObj("post");

	//3Dオブジェクト生成
	objPost = Object3d::Create();
	objPost->SetModel(modelPost);

	//音声読み込み
	//Audio::GetInstance()->SoundLoadWave("Alarm01.wav");

	//音声再生
	//audio->SoundPlayWave("Alarm01.wav", true);

	//カメラの初期化
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//カメラを3Dオブジェットにセット
	Object3d::SetCamera(camera);

	//camera->SetEye({ 0, 3.0f, -7.0f });
	camera->SetTarget({ 0,2.5f,0 });
	camera->SetDistance(8.0f);
	//モデル名を指定してファイルを読み込む
	fbxmodel1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

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
	delete postEffect;
	delete fbxobject1;
	delete fbxmodel1;
	delete camera;
	//スプライト個別解放
	delete sprite1;
	delete sprite;
	//3Dモデル解放
	delete modelPost;
	//3Dオブジェクト解放
	delete objPost;
}

void GamePlayScene::Update()
{
	Input* input = Input::GetInstance();
	//objPost->SetPosition({ 0, 15,10 });
	//objPost->SetRotation({ -45, 0, 0 });
	fbxobject1->SetRotation({ 0, 90, 0 });
	if (input->PushKey(DIK_SPACE))     // スペースキーが押されていたら
	{
		fbxobject1->PlayAnimation();
	}

	//X座標,Y座標を指定して表示
	//DebugText::GetInstance()->Print("Hello,DirectX!!", 0, 0);
	//X座標,Y座標,縮尺を指定して表情
	//DebugText::GetInstance()->Print("Nihon Kogakuin", 0, 20, 2.0f);
	
	//更新
	objPost->Update();
	//objChr->Update();
	camera->Update();
	fbxobject1->Update();
	/*sprite->Update();
	sprite1->Update();*/
}

void GamePlayScene::Draw(DirectXCommon* dxCommon)
{
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCmdList());
	// 背景スプライト描画
	sprite->Draw();
	sprite1->Draw();
	//スプライト描画
	postEffect->Draw(dxCommon->GetCmdList());

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma region 3D描画
	//3Dオブジェクト描画前処理
	Object3d::PreDraw();

	//3Dオブジェクトの描画
	//objPost->Draw();

	fbxobject1->Draw(dxCommon->GetCmdList());
	//3Dオブジェクト描画後処理
	Object3d::PostDraw();
}