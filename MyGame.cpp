#include "MyGame.h"

void MyGame::Initialize()
{
	//WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	//DirectXの初期化
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);

	//デバックテキスト
	debugText = new DebugText();
	//デバックテキスト用のテクスチャ番号を指定
	const int debugTextTexNumber = 2;
	//デバックテキスト用のテクスチャ読み込み
	spriteCommon->LoadTexture(debugTextTexNumber, L"Resources/debugfont.png");
	//デバックテキスト初期化
	debugText->Initialize(spriteCommon, debugTextTexNumber);

	//入力の初期化
	input = new Input();
	input->Initialize(winApp);

	//オーディオの初期化
	audio = new Audio();
	audio->Initialize();

	//3Dオブジェット静的初期化
	Object3d::StaticInitialize(dxCommon->GetDev(), winApp->window_width, winApp->window_height);
	//カメラの初期化
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//カメラを3Dオブジェットにセット
	Object3d::SetCamera(camera);

#pragma region シーン初期化処理
	//スプライト共通テクスチャ読み込み
	spriteCommon->LoadTexture(0, L"Resources/texture.png");
	spriteCommon->LoadTexture(1, L"Resources/house.png");

	//スプライトの生成
	Sprite* sprite = Sprite::Create(spriteCommon, 0);
	sprites.push_back(sprite);
	sprite->SetPosition({ 500,300,0 });

	//OBJからモデルデータを読み込む
	modelPost = Model::LoadFromObj("post");

	//3Dオブジェクト生成
	objPost = Object3d::Create();
	objPost->SetModel(modelPost);

	//音声読み込み
	//audio->SoundLoadWave("Alarm01.wav");

	//音声再生
	//audio->SoundPlayWave("Alarm01.wav", true);

	objPost->SetPosition({ -10,0,-5 });
	//objChr->SetPosition({ +10,0,+5 });

	camera->SetTarget({ -10,0,-5 });
	camera->SetEye({ 0, 0, -50 });
}

void MyGame::Finalize()
{
	winApp->Finalize();
#pragma endregion WindowsAPI後始末
	//各種解放
	//デバックテキスト解放
	delete debugText;
	//スプライト個別解放
	for (auto& sprite : sprites)
	{
		delete sprite;
	}
	//delete sprite;
	//スプライト共通部解放
	delete spriteCommon;
	//3Dモデル解放
	delete modelPost;
	//3Dオブジェクト解放
	delete objPost;
	//オーディオ解放
	delete audio;
	//DirectX解放
	delete dxCommon;
	//入力開放
	delete input;
	//WindowsAPI解放
	delete winApp;

}

void MyGame::Update()
{
#pragma region ウィンドウメッセージ処理
	if (winApp->ProcessMessage()) {
		//ゲームループを抜ける
		this->endRequst = true;
		return;
	}

	input->Update();

	if (input->PushKey(DIK_SPACE))     // スペースキーが押されていたら
	{

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
	//objChr->Update();
	camera->Update();
	//sprite->Update();
}

void MyGame::Draw()
{
	dxCommon->PreDraw();

	//3Dオブジェクト描画前処理
	Object3d::PreDraw(dxCommon->GetCmdList());
	//3Dオブジェクトの描画
	objPost->Draw();

	//3Dオブジェクト描画後処理
	Object3d::PostDraw();

	//スプライト共通コマンド
	spriteCommon->PreDraw();

	//スプライト描画
	//sprite->Draw();

	//デバックテキスト描画
	debugText->DrawAll();

	//描画後処理
	dxCommon->PostDraw();

}
