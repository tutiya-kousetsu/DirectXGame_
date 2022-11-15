#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"

#include "Enemy/EnemyBullet.h"

#include <fstream>
#include <cassert>
void GamePlayScene::Initialize(DirectXCommon* dxCommon)
{
	if (!Sprite::LoadTexture(1, L"Resources/gamePlay.png")) {
		assert(0);
		return;
	}
	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	//スプライトの生成
	Sprite::LoadTexture(2, L"Resources/sosa_sinan.png");
	sprite = Sprite::Create(2, { 0,0 });
	sprite->SetPosition({ 0,0 });
	Sprite::LoadTexture(3, L"Resources/Life.png");
	LifeSprite = Sprite::Create(3, { 0,0 });
	Sprite::LoadTexture(4, L"Resources/Life.png");
	LifeSprite2 = Sprite::Create(4, { 26,0 });
	Sprite::LoadTexture(5, L"Resources/Life.png");
	LifeSprite3 = Sprite::Create(5, { 52,0 });
	// テクスチャ2番に読み込み
	/*Sprite::LoadTexture(2, L"Resources/tex1.png");
	sprite = Sprite::Create(2, { 0.0f,0.0f });*/


	//ポストエフェクトの初期化
	for (int i = 0; i <= 1; i++) {
		postEffect[i] = new PostEffect();
	}
	//シェーダーの挿入
	postEffect[0]->Initialize(L"Resources/shaders/PostEffectPS.hlsl");

	postEffect[1]->Initialize(L"Resources/shaders/PixelShader.hlsl");


	//音声読み込み
	//Audio::GetInstance()->SoundLoadWave("Alarm01.wav");

	//音声再生
	//audio->SoundPlayWave("Alarm01.wav", true);

	//カメラの初期化
	camera.reset(new FollowingCamera());
	//カメラを3Dオブジェットにセット
	Object3d::SetCamera(camera.get());


	//デバイスをセット
	Fbx_Object3d::SetDevice(dxCommon->GetDev());
	//カメラをセット
	Fbx_Object3d::SetCamera(camera.get());
	//グラフィックスパイプライン生成
	Fbx_Object3d::CreateGraphicsPipeline();

	//乱数の初期化
	srand((unsigned)time(NULL));

	player.reset(new Player());
	floor = new Floor();
	playerBullet = new PlayerBullet();
	enemyBullet = new EnemyBullet();
	obstacle = new Obstacle();
	for (auto i = 0; i < 14; i++) {
		enemy[i] = new Enemy();
		enemy[i]->Initialize();
	}

	//データ読み込み
	skyModel = Model::LoadFromObj("skydome");
	skyObj = Object3d::Create();
	skyObj->SetModel(skyModel);

}

void GamePlayScene::Finalize()
{

	//スプライト個別解放
	delete sprite;
	delete LifeSprite;
	delete LifeSprite2;
	delete LifeSprite3;

	//3Dモデル解放
	//3Dオブジェクト解放
	delete postEffect[0];
	delete postEffect[1];

	//3Dオブジェクト解放
	delete skyModel;
	delete skyObj;
	delete playerBullet;
	delete enemyBullet;
	for (auto i = 0; i < 14; i++) {
		delete enemy[i];
	}
}

void GamePlayScene::Update()
{
	Input* input = Input::GetInstance();

	//X座標,Y座標を指定して表示
	//DebugText::GetInstance()->Print("Hello,DirectX!!", 0, 0);
	//X座標,Y座標,縮尺を指定して表情
	//DebugText::GetInstance()->Print("Nihon Kogakuin", 0, 20, 2.0f);
	// マウスを表示するかどうか(TRUEで表示、FALSEで非表示)
	ShowCursor(FALSE);
	// 座標の変更を反映
	SetCursorPos(960, 540);
	camera->SetFollowingTarget(player.get());
	//更新
	camera->Updata();
	player->Updata();
	floor->Updata();
	//playerBullet->Update();


	for (auto i = 0; i < 9; i++) {
		//enemy[i]->Updata();

	}

	if (!eneFlag[0]) {
		enemy[0]->Updata();
	}
	if (eneFlag[0]) {
		enemy[1]->Updata();
		enemy[2]->Updata();
	}
	if (eneFlag[1] && eneFlag[2]) {
		enemy[3]->Updata();
		enemy[4]->Updata();
		enemy[5]->Updata();
	}
	if (eneFlag[3] && eneFlag[4] && eneFlag[5]) {
		enemy[6]->Updata();
		enemy[7]->Updata();
		enemy[8]->Updata();
		enemy[9]->Updata();
	}
	if (eneFlag[6] && eneFlag[7] && eneFlag[8] && eneFlag[9]) {
		enemy[10]->Updata();
		enemy[11]->Updata();
		enemy[12]->Updata();
		enemy[13]->Updata();
		enemy[14]->Updata();
	}


	skyObj->Updata();
	obstacle->Updata();

	CheckAllCollision();
}

void GamePlayScene::Draw(DirectXCommon* dxCommon)
{
	//描画前処理
	dxCommon->PreDraw();
	//スプライト描画
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCmdList());
	//背景スプライト描画
	spriteBG->Draw();


	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer(dxCommon->GetCmdList());
#pragma endregion

#pragma endregion

	//3Dオブジェクト描画前処理
	Object3d::PreDraw();
	player->Draw();
	//playerBullet->Draw();
	for (auto i = 0; i < 9; i++) {
		//enemy[i]->Draw();

	}
	if (!eneFlag[0]) {
		enemy[0]->Draw();
	}
	if (eneFlag[0]) {
		enemy[1]->Draw();
		enemy[2]->Draw();
	}
	if (eneFlag[1] && eneFlag[2]) {
		enemy[3]->Draw();
		enemy[4]->Draw();
		enemy[5]->Draw();
	}
	if (eneFlag[3] && eneFlag[4] && eneFlag[5]) {
		enemy[6]->Draw();
		enemy[7]->Draw();
		enemy[8]->Draw();
		enemy[9]->Draw();
	}
	if (eneFlag[6] && eneFlag[7] && eneFlag[8] && eneFlag[9]) {
		enemy[10]->Draw();
		enemy[11]->Draw();
		enemy[12]->Draw();
		enemy[13]->Draw();
		enemy[14]->Draw();
	}

	//obstacle->Draw();
	skyObj->Draw();
	//groundObj->Draw();
	floor->Draw();
	Object3d::PostDraw();

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCmdList());

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite->Draw();
	if (playerLife >= 3){ LifeSprite3->Draw(); }
	if (playerLife >= 2) { LifeSprite2->Draw(); }
	if (playerLife >= 1) { LifeSprite->Draw(); }
	//spriteBG->Draw();
	// デバッグテキストの描画
	//debugText->DrawAll(cmdList);

	// スプライト描画後処理

	Sprite::PostDraw();

	//描画後処理
	dxCommon->PostDraw();

}


void GamePlayScene::CheckAllCollision()
{
	//判定の対象
	XMFLOAT3 posA, posB;
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullet();
	for (auto i = 0; i < 14; i++) {

		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy[i]->GetBullet();
#pragma region 自弾と敵の当たり判定
		//if (enemy[i]->GetFrameFlag()) {
		posA = enemy[i]->GetPosition();

		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			bulFlag = bullet->GetAlive();
			if (bulFlag) {
				posB = bullet->GetPosition();

				float dx = abs(posB.x - posA.x);
				float dy = abs(posB.y - posA.y);
				float dz = abs(posB.z - posA.z);

				if (dx < 1 && dy < 1 && dz < 1) {
					enemy[i]->OnCollision();
					bullet->OnCollision();
					gameScore++;
					eneFlag[i] = true;
				}
			}
		}
#pragma endregion

#pragma region 敵と自機の当たり判定
		posA = enemy[i]->GetPosition();

		posB = player->GetPosition();

		float dx = abs(posB.x - posA.x);
		float dy = abs(posB.y - posA.y);
		float dz = abs(posB.z - posA.z);

		if (dx < 1 && dy < 1 && dz < 1) {
			//enemy[i]->Hit();
			enemy[i]->OnCollision();
			player->OnCollision();
		}
#pragma endregion

#pragma region 敵弾と自機の当たり判定
		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {

			bulFlag = bullet->GetAlive();
			if (bulFlag) {
				posA = player->GetPosition();

				posB = bullet->GetPosition();

				float dx = abs(posB.x - posA.x);
				float dy = abs(posB.y - posA.y);
				float dz = abs(posB.z - posA.z);

				if (dx < 1 && dy < 1 && dz < 1) {
					bullet->OnCollision();
					player->OnCollision();
					playerLife--;
				}
			}
		}
#pragma endregion

	}

#pragma region yukaと自機の当たり判定
	//posA = floor->GetPosition();

	//posB = player->GetPosition();

	//float dx = abs(posB.x - posA.x);
	//float dy = abs(posB.y - posA.y);
	//float dz = abs(posB.z - posA.z);

	//if (dx < 1 && dy < 1 && dz < 1) {
	//	//player->FloorCollision();
	//}
#pragma endregion

	playerPos = player->GetPosition();
	player->SetPosition(playerPos);
	//プレイヤーのHPが0になったら画面切り替え
	if (playerLife == 0 || playerPos.y <= -5) {
		//シーン切り替え
		BaseScene* scene = new GameOver();
		this->sceneManager->SetNextScene(scene);
	}
	if (gameScore >= 15) {
		//シーン切り替え
		BaseScene* scene = new GameClear();
		this->sceneManager->SetNextScene(scene);
	}
}

//void GamePlayScene::LoadEnemyPopData()
//{
//	//ファイルを開く
//	std::ifstream file;
//	file.open("Resources/enemyPop.csv");
//	assert(file.is_open());
//
//	//ファイルの内容を文字列ストリームにコピー
//	enemyPopCommands << file.rdbuf();
//
//	//ファイルを閉じる
//	file.close();
//}

//void GamePlayScene::UpdataEnemyPopCommand()
//{
//	//1行分の文字行列を入れる変数
//	std::string line;
//	//コマンド実行ループ
//	while (getline(enemyPopCommands, line)) {
//		//1行分の文字列をストリームに変換して解析しやすくする
//		std::istringstream line_stream(line);
//
//		std::string word;
//		//,区切りで行の先頭文字を取得
//		getline(line_stream, word, ',');
//
//		//"//"から始まる行はコメント
//		if (word.find("//") == 0) {
//			//コメントの行を飛ばす
//			continue;
//		}
//
//		//POPコマンド
//		if (word.find("POP") == 0) {
//			//x座標
//			getline(line_stream, word, ',');
//			float x = (float)std::atof(word.c_str());
//
//			//y座標
//			getline(line_stream, word, ',');
//			float y = (float)std::atof(word.c_str());
//			
//			//z座標
//			getline(line_stream, word, ',');
//			float z = (float)std::atof(word.c_str());
//
//			//敵を発生させる
//
//		}
//	}
//}