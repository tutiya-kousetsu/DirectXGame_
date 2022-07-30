#include "Framework.h"
#include "FbxLoader.h"

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


	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);


	//デバックテキスト用のテクスチャ番号を指定
	const int debugTextTexNumber = 2;
	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
		return;
	}
	//デバックテキスト
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextTexNumber);

	//入力の初期化
	input = Input::GetInstance();
	input->Initialize(winApp);


	//オーディオの初期化
	/*audio = Audio::GetInstance();
	audio->Initialize();*/

	//3Dオブジェット静的初期化
	Object3d::StaticInitialize(dxCommon->GetDev(),dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);
	
}

void Framework::Finalize()
{
	//シングルトンにしたらdeleteダメ
	//DirectX解放
	delete dxCommon;
	//WindowsAPIの終了処理
	winApp->Finalize();
	//WindowsAPI解放
	delete winApp;
	//FBXの解放
	FbxLoader::GetInstance()->Finalize();

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
	/// <summary>
	/// シーンの更新
	/// </summary>
	SceneManager::GetInstance()->Update(dxCommon);
}

void Framework::Draw()
{
	//シーン描画
	SceneManager::GetInstance()->Draw(dxCommon);
	//デバックテキスト描画
	//debugText->DrawAll();
}
