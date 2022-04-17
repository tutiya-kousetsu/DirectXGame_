#include "Framework.h"

void Framework::Run()
{
	//ゲームの初期化
	Initialize();

	while (true)  // ゲームループ
	{
		//毎フレーム更新
		Update();
		//終了リクエストが来たら抜ける
		if (IsEndRequst()) {
			break;
		}
		//描画
		Draw();
	}
	// ゲームの終了
	Finalize();
}

void Framework::Initialize()
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

}

void Framework::Finalize()
{
	//デバックテキスト解放
	delete debugText;
	//スプライト共通部解放
	delete spriteCommon;
	//オーディオ解放
	delete audio;
	//DirectX解放
	delete dxCommon;
	//入力開放
	delete input;
	//WindowsAPIの終了処理
	winApp->Finalize();
	//WindowsAPI解放
	delete winApp;

}

void Framework::Update()
{
#pragma region ウィンドウメッセージ処理
	if (winApp->ProcessMessage()) {
		//ゲームループを抜ける
		this->endRequst = true;
		return;
	}
	//入力の更新
	input->Update();

}

void Framework::Draw()
{
}
