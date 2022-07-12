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

	FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());

	//スプライト共通部分の初期化
	spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->Initialize(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);

	//デバックテキスト
	debugText = DebugText::GetInstance();
	//デバックテキスト用のテクスチャ番号を指定
	const int debugTextTexNumber = 2;
	//デバックテキスト用のテクスチャ読み込み
	spriteCommon->LoadTexture(debugTextTexNumber, L"Resources/debugfont.png");
	//デバックテキスト初期化
	debugText->Initialize(spriteCommon, debugTextTexNumber);

	//入力の初期化
	input = Input::GetInstance();
	input->Initialize(winApp);

	SpriteCommon::GetInstance()->LoadTexture(100, L"Resources/white1x1.png");
	postEffect = new PostEffect();
	postEffect->Initialize();

	//オーディオの初期化
	/*audio = Audio::GetInstance();
	audio->Initialize();*/

	//3Dオブジェット静的初期化
	Object3d::StaticInitialize(dxCommon->GetDev(),dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);
	//シーンマネーギャーの生成
	this->sceneManager = new SceneManager();
}

void Framework::Finalize()
{
	delete postEffect;
	//シーンマネージャー解放
	delete sceneManager;
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
	this->sceneManager->Update(dxCommon);
}

void Framework::Draw()
{
	//描画前処理
	dxCommon->PreDraw();

	//シーン描画
	this->sceneManager->Draw(dxCommon);
	postEffect->Draw(dxCommon->GetCmdList());
	//デバックテキスト描画
	//debugText->DrawAll();

	//描画後処理
	dxCommon->PostDraw();
}
