#include<vector>

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Object3d.h"
#include "Model.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "Audio.h"

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int)
{
#pragma region WindowsAPI初期化

	WinApp* winApp = nullptr;

	winApp = new WinApp();
	winApp->Initialize();

	MSG msg{};  // メッセージ
#pragma endregion WindowsAPI初期化

#pragma region DirectX初期化処理

	DirectXCommon* dxCommon = nullptr;

	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	Input* input = nullptr;
	input = new Input();
	input->Initialize(winApp);

	Object3d::StaticInitialize(dxCommon->GetDev(), winApp->window_width, winApp->window_height);
	DebugCamera* camera = nullptr;
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	Object3d::SetCamera(camera);

#pragma endregion DirectX初期化処理

#pragma region 描画初期化処理

	SpriteCommon* spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);

	spriteCommon->LoadTexture(0, L"Resources/texture.png");
	spriteCommon->LoadTexture(1, L"Resources/house.png");

	//デバックテキスト
	DebugText* debugText = nullptr;
	debugText = new DebugText();
	//デバックテキスト用のテクスチャ番号を指定
	const int debugTextTexNumber = 2;
	//デバックテキスト用のテクスチャ読み込み
	spriteCommon->LoadTexture(debugTextTexNumber, L"Resources/debugfont.png");
	//デバックテキスト初期化
	debugText->Initialize(spriteCommon, debugTextTexNumber);

	//スプライト
	std::vector<Sprite*> sprites;
	/*Sprite* sprite = Sprite::Create(spriteCommon, 0);
	sprites.push_back(sprite);
	sprite->SetPosition({ 500,300,0 });*/

	for (int i = 0; i < 20; i++)
	{
		int texNum = rand() % 2;

		Sprite* sprite = Sprite::Create(spriteCommon, texNum, { 0,0 }, false, false);

		sprite->SetPosition({ (float)(rand() % 1280),(float)(rand() % 720),0 });

		//sprite->SetRotation((float)(rand() % 360));

		sprite->SetSize({ (float)(rand() % 400), (float)(rand() % 100) });

		sprite->TransferVertexBuffer();

		sprites.push_back(sprite);
		//sprite->SetPosition({ 500,300,0 });
	}

	//モデル読み込み
	Model* modelPost = Model::LoadFromObj("post");
	Model* modelChr = Model::LoadFromObj("post");

	//3Dオブジェクト生成
	Object3d* objPost = Object3d::Create();
	Object3d* objChr = Object3d::Create();


	objPost->SetModel(modelPost);
	objChr->SetModel(modelChr);

	objPost->SetPosition({ -10,0,-5 });
	objChr->SetPosition({ +10,0,+5 });

	camera->SetTarget({ -10,0,-5 });
	camera->SetEye({ 0, 0, 0 });

#pragma endregion 描画初期化処理

	int counter = 0; // アニメーションの経過時間カウンター

	while (true)  // ゲームループ
	{
#pragma region ウィンドウメッセージ処理
		if (winApp->ProcessMessage()) {
			break;
		}
#pragma endregion ウィンドウメッセージ処理

#pragma region DirectX毎フレーム処理
		// DirectX毎フレーム処理　ここから

		input->Update();

		const int cycle = 540; // 繰り返しの周期
		counter++;
		counter %= cycle; // 周期を超えたら0に戻る
		float scale = (float)counter / cycle; // [0,1]の数値

		scale *= 360.0f;
		objPost->SetModel(modelPost);
		objChr->SetModel(modelChr);

		if (input->PushKey(DIK_SPACE))     // スペースキーが押されていたら
		{
			// 画面クリアカラーの数値を書き換える
			objPost->SetModel(modelChr);
			objChr->SetModel(modelChr);
		}

		// 座標操作
		if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
		{

		}

		//X座標,Y座標を指定して表示
		debugText->Print("Hello,DirectX!!", 200, 100);
		//X座標,Y座標,縮尺を指定して表情
		debugText->Print("Nihon Kogakuin", 200, 200, 2.0f);

		//更新
		objPost->Update();
		objChr->Update();
		camera->Update();
		for (auto& sprite : sprites)
		{
			sprite->Update();
		}

		// DirectX毎フレーム処理　ここまで
#pragma endregion DirectX毎フレーム処理

#pragma region グラフィックスコマンド

		dxCommon->PreDraw();

		Object3d::PreDraw(dxCommon->GetCmdList());
		objPost->Draw();
		objChr->Draw();
		Object3d::PostDraw();

		spriteCommon->PreDraw();
		for (auto& sprite : sprites)
		{
			sprite->Draw();
		}

		//デバックテキスト描画
		debugText->DrawAll();
		// ４．描画コマンドここまで
		dxCommon->PostDraw();
	}
	// XAudio2解放
   // xAudio2.Reset();
	// 音声データ解放
   // SoundUnload(&soundData1);


#pragma region WindowsAPI後始末
	winApp->Finalize();
#pragma endregion WindowsAPI後始末
	//各種解放
	delete input;
	delete winApp;
	delete spriteCommon;
	for (auto& sprite : sprites)
	{
		delete sprite;
	}
	//delete sprite;
	delete modelPost;
	delete modelChr;
	delete objChr;
	delete objPost;
	delete debugText;
	return 0;
}