#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"

void GamePlayScene::Initialize(DirectXCommon* dxCommon)
{
	if (!Sprite::LoadTexture(1, L"Resources/background.png")) {
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
	
	//FBXの初期化
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

	camera->SetEye({ 0, 3.0f, -7.0f });
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
	delete postEffect[0];
	delete postEffect[1];
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
	postEffect[0]->PreDrawScene(dxCommon->GetCmdList());
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

	// デバッグテキストの描画
	//debugText->DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion

	//3Dオブジェクト描画前処理
	Object3d::PreDraw();

	//3Dオブジェクトの描画
	//objPost->Draw();

	fbxobject1->Draw(dxCommon->GetCmdList());
	//3Dオブジェクト描画後処理
	Object3d::PostDraw();

	postEffect[0]->PostDrawScene(dxCommon->GetCmdList());

	postEffect[1]->PreDrawScene(dxCommon->GetCmdList());
	postEffect[0]->Draw(dxCommon->GetCmdList());
	postEffect[1]->PostDrawScene(dxCommon->GetCmdList());

	//描画前処理
	dxCommon->PreDraw();

	postEffect[1]->Draw(dxCommon->GetCmdList());

	//描画後処理
	dxCommon->PostDraw();

}