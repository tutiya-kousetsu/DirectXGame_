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
	//SpriteCommon::GetInstance()->LoadTexture(1, L"Resources/gamePlay.png");
	if (!Sprite::LoadTexture(1, L"Resources/background.png")) {
		assert(0);
		return;
	}
	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	//スプライトの生成
	//sprite = Sprite::Create(1, { 0,0 }, false, false);
	//sprite->SetPosition({ 0,0 });

	//SpriteCommon::GetInstance()->LoadTexture(2, L"Resources/tex1.png");
	//スプライトの生成
	//sprite1 = Sprite::Create(2, { 0,0 }, false, false);
	// テクスチャ2番に読み込み
	Sprite::LoadTexture(2, L"Resources/tex1.png");
	sprite = Sprite::Create(2, { 0.0f,0.0f });

	//sprite1->SetPosition({ 0,0 });

	//SpriteCommon::GetInstance()->LoadTexture(100, L"Resources/white1x1.png");
	//Sprite::LoadTexture(100, L"Resources/white1280x720.png");
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
	camera->SetDistance(-45.0f);
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
	delete postEffect;
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

	//if (input->PushKey(DIK_SPACE))     // スペースキーが押されていたら
	//{
	//	fbxobject1->PlayAnimation();
	//}
	//X座標,Y座標を指定して表示
	//DebugText::GetInstance()->Print("Hello,DirectX!!", 0, 0);
	//X座標,Y座標,縮尺を指定して表情
	//DebugText::GetInstance()->Print("Nihon Kogakuin", 0, 20, 2.0f);

	//更新
	objPost->Update();
	//objChr->Update();
	camera->Update();
	fbxobject1->Update();
}

void GamePlayScene::Draw(DirectXCommon* dxCommon)
{
	//スプライト共通コマンド
	//SpriteCommon::GetInstance()->PreDraw();
	//スプライト描画
#pragma region 背景スプライト描画
// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCmdList());
	// 背景スプライト描画
	//spriteBG->Draw();
	//sprite1->Draw();
	

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer(dxCommon->GetCmdList());
#pragma endregion


	#pragma region 前景スプライト描画
// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCmdList());

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite->Draw();
	postEffect->Draw(dxCommon->GetCmdList());

	// デバッグテキストの描画
	//debugText->DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion


	postEffect->PreDrawScene(dxCommon->GetCmdList());

	//3Dオブジェクト描画前処理
	Object3d::PreDraw();

	//3Dオブジェクトの描画
	//objPost->Draw();

	fbxobject1->Draw(dxCommon->GetCmdList());
	//3Dオブジェクト描画後処理
	Object3d::PostDraw();

	postEffect->PostDrawScene(dxCommon->GetCmdList());


}