#include "MyGame.h"

void MyGame::Initialize()
{
	//基底クラスの初期化
	Framework::Initialize();

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
	//スプライト個別解放
	for (auto& sprite : sprites)
	{
		delete sprite;
	}
	//delete sprite;
	//3Dモデル解放
	delete modelPost;
	//3Dオブジェクト解放
	delete objPost;
	//基底クラスの終了処理
	Framework::Finalize();
}

void MyGame::Update()
{
	//基底クラスの更新処理
	Framework::Update();

	if (input->PushKey(DIK_SPACE))     // スペースキーが押されていたら
	{

	}

	// 座標操作
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{

	}

	//X座標,Y座標を指定して表示
	debugText->Print("Hello,DirectX!!", 0, 0);
	//X座標,Y座標,縮尺を指定して表情
	debugText->Print("Nihon Kogakuin", 0, 20, 2.0f);

	//更新
	objPost->Update();
	//objChr->Update();
	camera->Update();
	//sprite->Update();
}

void MyGame::Draw()
{
	//描画前処理
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
