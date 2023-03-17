#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
#include "Player.h"
#include "Enemy/EnemyBullet.h"
#include "Collision.h"
#include "ParticleManager.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "MeshCollider.h"
#include "TouchableObject.h"
#include <fstream>
#include <cassert>

void GamePlayScene::Initialize(DirectXCommon* dxCommon)
{
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
	Sprite::LoadTexture(6, L"Resources/Life.png");
	LifeSprite4 = Sprite::Create(6, { 78,0 });
	Sprite::LoadTexture(7, L"Resources/Life.png");
	LifeSprite5 = Sprite::Create(7, { 104,0 });
	Sprite::LoadTexture(8, L"Resources/Life.png");
	LifeSprite6 = Sprite::Create(8, { 130,0 });
	Sprite::LoadTexture(9, L"Resources/Life.png");
	LifeSprite7 = Sprite::Create(9, { 156,0 });
	Sprite::LoadTexture(10, L"Resources/Life.png");
	LifeSprite8 = Sprite::Create(10, { 182,0 });
	//ポストエフェクトの初期化
	for (int i = 0; i <= 1; i++) {
		postEffect[i] = new PostEffect();
	}
	//シェーダーの挿入
	postEffect[0]->Initialize(L"Resources/shaders/PostEffectPS.hlsl");

	postEffect[1]->Initialize(L"Resources/shaders/PixelShader.hlsl");

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

	//自機弾の初期化
	playerBullet = new PlayerBullet();
	//敵弾の初期化
	enemyBullet = new EnemyBullet();
	//当たり判定のインスタンス
	collisionMan = CollisionManager::GetInstance();
	//自機のオブジェクトセット+初期化
	player = player->Create(Model::CreateFromOBJ("octopus"));

	//床のオブジェクト生成
	floorModel = Model::CreateFromOBJ("FloorBox");
	floor = TouchableObject::Create(floorModel);
	floor->SetScale({ 20.f, 5.0f, 20.f });
	floor->SetPosition({ 0,-18.5f,0 });
	//パーティクのインスタンス
	particleMan = ParticleManager::GetInstance();
	particleMan->Initialize();
	particleMan->SetCamera(camera.get());
	//for (int i = 0; i < 11; i++) {
	//	//前
	//	frontEnemy[i] = new FrontEnemy();
	//	frontEnemy[i]->Initialize();
	//	//敵に自機のアドレスを渡して敵が自機を使えるようにする
	//	frontEnemy[i]->SetPlayer(player);
	//}
	for (int i = 0; i < 7; i++) {
		//左
		leftEnemy[i] = new LeftEnemy();
		leftEnemy[i]->Initialize();
		//敵に自機のアドレスを渡して敵が自機を使えるようにする
		leftEnemy[i]->SetPlayer(player);
	}
	for (int i = 0; i < 4; i++) {
		//右
		rightEnemy[i] = new RightEnemy();
		rightEnemy[i]->Initialize();
		//敵に自機のアドレスを渡して敵が自機を使えるようにする
		rightEnemy[i]->SetPlayer(player);
	}
	for (int i = 0; i < 2; i++) {
		//後ろ
		backEnemy[i] = new BackEnemy();
		backEnemy[i]->Initialize();
		//敵に自機のアドレスを渡して敵が自機を使えるようにする
		backEnemy[i]->SetPlayer(player);
	}
	//データ読み込み
	skyModel = Model::CreateFromOBJ("skydome");
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
	delete LifeSprite4;
	delete LifeSprite5;
	delete LifeSprite6;
	delete LifeSprite7;
	delete LifeSprite8;
	//3Dオブジェクト解放
	delete skyModel;
	delete floor;
	delete player;
	delete skyObj;
	for (int i = 0; i < 7; i++) {
		delete leftEnemy[i];
	}
	for (int i = 0; i < 4; i++) {
		delete rightEnemy[i];
	}
	for (int i = 0; i < 2; i++) {
		delete backEnemy[i];
	}
}

void GamePlayScene::Update()
{
	Input* input = Input::GetInstance();

	// マウスを表示するかどうか(TRUEで表示、FALSEで非表示)
	ShowCursor(FALSE);
	// 座標の変更を反映
	SetCursorPos(960, 540);

	camera->SetFollowingTarget(player);

	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();
	float dy = mouseMove.lX * scaleY;
	angleY = -dy * XM_PI;

	{
		// 追加回転分の回転行列を生成
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationY(-angleY);
		// 累積の回転行列を合成
		matRot = matRotNew * matRot;

		// 注視点から視点へのベクトルと、上方向ベクトル
		XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 0.5f, 0.0f, 0.0f };

		// ベクトルを回転
		vTargetEye = XMVector3Transform(vTargetEye, matRot);
		vUp = XMVector3Transform(vUp, matRot);

		// 長さ
		float length = 0.0f;

		XMFLOAT3 target1 = camera->GetTarget();
		camera->SetEye({ target1.x + vTargetEye.m128_f32[0], target1.y + vTargetEye.m128_f32[1], target1.z + vTargetEye.m128_f32[2] });
		camera->SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });

		// 注視点からずらした位置に視点座標を決定
		XMFLOAT3 target2 = camera->GetTarget();
		XMFLOAT3 eye = camera->GetEye();

		XMFLOAT3 fTargetEye = { 0.0f, 0.0f, 0.0f };
		XMVECTOR vecF = XMLoadFloat3(&fTargetEye);
		// FLOAT3に変換
		XMStoreFloat3(&fTargetEye, vecF);
		XMVECTOR vecTarget = XMLoadFloat3(&target2);
		// FLOAT3に変換
		XMStoreFloat3(&target2, vecTarget);
		XMVECTOR vecEye = XMLoadFloat3(&eye);
		// FLOAT3に変換
		XMStoreFloat3(&eye, vecEye);
		//正規化
		fTargetEye.x = eye.x - target2.x;
		fTargetEye.y = eye.y - target2.y;
		fTargetEye.z = eye.z - target2.z;

		//プレイヤーの回転
		XMFLOAT3 playerRot = player->GetRotation();
		playerRot.y = atan2f(-fTargetEye.x, -fTargetEye.z);
		playerRot.y *= 180 / XM_PI;
		player->SetRotation({ 0.0f, playerRot.y, 0.0f });
	}

	//プレイヤーの更新
	player->Update();

	LoadFrontEnemyPopData();
	UpdataFrontEnemyPopCommand();
	for (auto& front : frontEnemy) {
		front->Update();
	}
	//カメラの更新
	camera->Update();
	//床の更新
	floor->Update();
	//敵の発生する順番
	if (fEnePhase >= 0) {
		//frontEnemy[0]->Update();
	}
	if (fEnePhase >= 1) {
		/*frontEnemy[1]->Update();
		frontEnemy[2]->Update();*/
	}
	if (fEnePhase >= 3 && lEnePhase >= 0) {
		/*frontEnemy[3]->Update();
		frontEnemy[4]->Update();*/
		leftEnemy[0]->Update();
	}
	if (fEnePhase >= 5 && lEnePhase >= 1) {
		/*frontEnemy[5]->Update();
		frontEnemy[6]->Update();*/
		leftEnemy[1]->Update();
		leftEnemy[2]->Update();
	}
	if (fEnePhase >= 7 && lEnePhase >= 3 && rEnePhase >= 0) {
		/*frontEnemy[7]->Update();
		frontEnemy[8]->Update();*/
		leftEnemy[3]->Update();
		leftEnemy[4]->Update();
		rightEnemy[0]->Update();
		rightEnemy[1]->Update();
	}
	if (fEnePhase >= 9 && lEnePhase >= 5 && rEnePhase >= 2) {
		/*frontEnemy[9]->Update();
		frontEnemy[10]->Update();*/
		leftEnemy[5]->Update();
		leftEnemy[6]->Update();
		rightEnemy[2]->Update();
		rightEnemy[3]->Update();
		backEnemy[0]->Update();
		backEnemy[1]->Update();
	}

	skyObj->Update();
	//障害物のマップチップ読み込み用
	LoadObstaclePopData();
	UpdataObstaclePopCommand();
	//当たり判定
	collisionMan->CheckAllCollisions();
	CheckAllCollision();

	particleMan->Update();
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
	//前敵
	/*for (int i = 0; i < 11; i++) {
		frontEnemy[i]->Draw();
	}*/
	for (auto& front : frontEnemy) {
		front->Draw();
	}
	//左敵
	for (int i = 0; i < 7; i++) {
		leftEnemy[i]->Draw();
	}
	//右敵
	for (int i = 0; i < 4; i++) {
		rightEnemy[i]->Draw();
	}
	//後ろ的
	for (int i = 0; i < 2; i++) {
		backEnemy[i]->Draw();
	}
	//障害物
	for (auto& obstacle : obstacles) {
		obstacle->Draw();
	}
	skyObj->Draw();
	floor->Draw();
	particleMan->Draw();
	Object3d::PostDraw();

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCmdList());

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite->Draw();
	if (playerLife >= 8) { LifeSprite8->Draw(); }
	if (playerLife >= 7) { LifeSprite7->Draw(); }
	if (playerLife >= 6) { LifeSprite6->Draw(); }
	if (playerLife >= 5) { LifeSprite5->Draw(); }
	if (playerLife >= 4) { LifeSprite4->Draw(); }
	if (playerLife >= 3) { LifeSprite3->Draw(); }
	if (playerLife >= 2) { LifeSprite2->Draw(); }
	if (playerLife >= 1) { LifeSprite->Draw(); }
	// スプライト描画後処理

	Sprite::PostDraw();

	//描画後処理
	dxCommon->PostDraw();

}

void GamePlayScene::LoadFrontEnemyPopData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/frontEnemyPop.csv");
	assert(file.is_open());

	//ファイル内容を文字列ストリームにコピー
	frontPopCom << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GamePlayScene::UpdataFrontEnemyPopCommand()
{
	if(waitFlag){
			//csv側のフェーズと敵フェーズが一致していたらWaitフラグをfalseにする
			if (fWaitPhase == fEnePhase) {
				waitFlag = false;
			}
			//一致していなかったらreturnで返す
			else {
				return;
			}
	}
	//1行分の文字列を入れる変数
	std::string line;
	//コマンド実行ループ
	while (getline(frontPopCom, line)) {
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行は飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0) {
			//x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			//y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			//z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//敵を発生させる
			//コンストラクタ呼ぶよ
			std::unique_ptr<FrontEnemy> newFront = std::make_unique<FrontEnemy>();
			newFront->Initialize(XMFLOAT3(x, y, z));
			newFront->SetPlayer(player);
			//障害物を登録する
			frontEnemy.push_back(std::move(newFront));
		}
		else if (word.find("FLAG") == 0) {
			//flag
			getline(line_stream, word, ',');
			int flag = atoi(word.c_str());

			waitFlag = true;
			fWaitPhase = flag;

			break;
		}
	}
}


void GamePlayScene::LoadObstaclePopData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/ObstaclePop.csv");
	assert(file.is_open());

	//ファイル内容を文字列ストリームにコピー
	obstaclePopCom << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GamePlayScene::UpdataObstaclePopCommand()
{
	//1行分の文字列を入れる変数
	std::string line;
	//コマンド実行ループ
	while (getline(obstaclePopCom, line)) {
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行は飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0) {
			//x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			//y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			//z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//敵を発生させる
			//コンストラクタ呼ぶよ
			std::unique_ptr<Obstacle> newObstacle = std::make_unique<Obstacle>();
			newObstacle->Initialize(XMFLOAT3(x, y, z));
			//障害物を登録する
			obstacles.push_back(std::move(newObstacle));
		}
	}
	for (auto& obstacle : obstacles) {
		obstacle->Update();
	}
}

//前敵の弾の当たり判定
void GamePlayScene::FrontColl()
{
	for (auto& front : frontEnemy) {
		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = front->GetBullet();
#pragma region 敵弾と自機の当たり判定
		for (auto& eb : enemyBullets) {
			Sphere eBullet;

			if (eb->GetAlive()) {
				eBullet.center = XMLoadFloat3(&eb->GetPosition());
				eBullet.radius = eb->GetScale().x;
				Sphere playerShape;
				playerShape.center = XMLoadFloat3(&player->GetPosition());
				playerShape.radius = player->GetScale().x;

				if (Collision::CheckSphere2Sphere(eBullet, playerShape)) {
					eb->OnCollision();
					player->CreateParticle();
					playerLife--;
				}
			}

#pragma endregion

#pragma region 敵弾と障害物の当たり判定
			if (eb->GetAlive()) {
				eBullet.center = XMLoadFloat3(&eb->GetPosition());
				eBullet.radius = eb->GetScale().x;
				if (front->GetAlive()) {
					Sphere obstacleShape;
					for (auto& ob : obstacles) {
						obstacleShape.center = XMLoadFloat3(&ob->GetPosition());
						obstacleShape.radius = ob->GetScale().x;

						if (Collision::CheckSphere2Sphere(eBullet, obstacleShape)) {
							eb->OnCollision();
						}
					}
				}
			}
		}
	}
#pragma endregion
}

//左敵の弾の当たり判定
void GamePlayScene::LeftColl()
{
	for (int i = 0; i < 7; i++) {
		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = leftEnemy[i]->GetBullet();
#pragma region 敵弾と自機の当たり判定
		for (auto& eb : enemyBullets) {
			Sphere eBullet;

			if (eb->GetAlive()) {
				eBullet.center = XMLoadFloat3(&eb->GetPosition());
				eBullet.radius = eb->GetScale().x;
				Sphere playerShape;
				playerShape.center = XMLoadFloat3(&player->GetPosition());
				playerShape.radius = player->GetScale().x;

				if (Collision::CheckSphere2Sphere(eBullet, playerShape)) {
					eb->OnCollision();
					player->CreateParticle();
					playerLife--;
				}
			}

#pragma endregion

#pragma region 敵弾と障害物の当たり判定
			if (eb->GetAlive()) {
				eBullet.center = XMLoadFloat3(&eb->GetPosition());
				eBullet.radius = eb->GetScale().x;
				if (leftEnemy[i]->GetAlive()) {
					Sphere obstacleShape;
					for (auto& ob : obstacles) {
						obstacleShape.center = XMLoadFloat3(&ob->GetPosition());
						obstacleShape.radius = ob->GetScale().x;

						if (Collision::CheckSphere2Sphere(eBullet, obstacleShape)) {
							eb->OnCollision();
						}
					}
				}
			}
		}
	}
#pragma endregion
}


//右敵の弾の当たり判定
void GamePlayScene::RightColl()
{
	for (int i = 0; i < 4; i++) {
		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = rightEnemy[i]->GetBullet();
#pragma region 敵弾と自機の当たり判定
		for (auto& eb : enemyBullets) {
			Sphere eBullet;

			if (eb->GetAlive()) {
				eBullet.center = XMLoadFloat3(&eb->GetPosition());
				eBullet.radius = eb->GetScale().x;
				Sphere playerShape;
				playerShape.center = XMLoadFloat3(&player->GetPosition());
				playerShape.radius = player->GetScale().x;

				if (Collision::CheckSphere2Sphere(eBullet, playerShape)) {
					eb->OnCollision();
					player->CreateParticle();
					playerLife--;
				}
			}

#pragma endregion

#pragma region 敵弾と障害物の当たり判定

			if (eb->GetAlive()) {
				eBullet.center = XMLoadFloat3(&eb->GetPosition());
				eBullet.radius = eb->GetScale().x;
				if (rightEnemy[i]->GetAlive()) {
					Sphere obstacleShape;
					for (auto& ob : obstacles) {
						obstacleShape.center = XMLoadFloat3(&ob->GetPosition());
						obstacleShape.radius = ob->GetScale().x;

						if (Collision::CheckSphere2Sphere(eBullet, obstacleShape)) {
							eb->OnCollision();
						}
					}
				}

			}
		}

#pragma endregion
	}
}

//後ろ敵の弾の当たり判定
void GamePlayScene::BackColl()
{
	for (int i = 0; i < 2; i++) {
		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = backEnemy[i]->GetBullet();
#pragma region 敵弾と自機の当たり判定
		for (auto& eb : enemyBullets) {
			Sphere eBullet;

			if (eb->GetAlive()) {
				eBullet.center = XMLoadFloat3(&eb->GetPosition());
				eBullet.radius = eb->GetScale().x;
				Sphere playerShape;
				playerShape.center = XMLoadFloat3(&player->GetPosition());
				playerShape.radius = player->GetScale().x;

				if (Collision::CheckSphere2Sphere(eBullet, playerShape)) {
					eb->OnCollision();
					player->CreateParticle();
					playerLife--;
				}
			}

#pragma endregion

#pragma region 敵弾と障害物の当たり判定


			if (eb->GetAlive()) {
				eBullet.center = XMLoadFloat3(&eb->GetPosition());
				eBullet.radius = eb->GetScale().x;
				if (backEnemy[i]->GetAlive()) {
					Sphere obstacleShape;
					for (auto& ob : obstacles) {
						obstacleShape.center = XMLoadFloat3(&ob->GetPosition());
						obstacleShape.radius = ob->GetScale().x;

						if (Collision::CheckSphere2Sphere(eBullet, obstacleShape)) {
							eb->OnCollision();
						}
					}
				}

			}
		}
	}
#pragma endregion

}

void GamePlayScene::CheckAllCollision()
{
	FrontColl();
	LeftColl();
	RightColl();
	BackColl();

	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullet();

#pragma region 自弾と敵の当たり判定

	Sphere pBullet;
	for (auto& pb : playerBullets) {
		if (pb->GetAlive()) {
			pBullet.center = XMLoadFloat3(&pb->GetPosition());
			pBullet.radius = pb->GetScale().x;

			//前の敵
			for (auto& front : frontEnemy) {
				if (front->GetAlive()) {
					Sphere fEnemyShape;
					fEnemyShape.center = XMLoadFloat3(&front->GetPosition());
					fEnemyShape.radius = front->GetScale().z;

					if (Collision::CheckSphere2Sphere(pBullet, fEnemyShape)) {
						pb->OnCollision();
						front->OnCollision();
						if (!front->GetAlive()) {
							fEnePhase++;
						}
					}
				}

			}
			for (int i = 0; i < 7; i++) {
				//左の敵
				if (leftEnemy[i]->GetAlive()) {
					Sphere lEnemyShape;
					lEnemyShape.center = XMLoadFloat3(&leftEnemy[i]->GetPosition());
					lEnemyShape.radius = leftEnemy[i]->GetScale().z;

					if (Collision::CheckSphere2Sphere(pBullet, lEnemyShape)) {
						pb->OnCollision();
						leftEnemy[i]->OnCollision();
						if (!leftEnemy[i]->GetAlive()) {
							lEnePhase++;
						}
					}
				}
			}
			for (int i = 0; i < 4; i++) {

				//右の敵
				if (rightEnemy[i]->GetAlive()) {
					Sphere lEnemyShape;
					lEnemyShape.center = XMLoadFloat3(&rightEnemy[i]->GetPosition());
					lEnemyShape.radius = rightEnemy[i]->GetScale().z;

					if (Collision::CheckSphere2Sphere(pBullet, lEnemyShape)) {
						pb->OnCollision();
						rightEnemy[i]->OnCollision();
						if (!rightEnemy[i]->GetAlive()) {
							rEnePhase++;
						}
					}
				}
			}
			for (int i = 0; i < 2; i++) {

				//後ろの敵
				if (backEnemy[i]->GetAlive()) {
					Sphere lEnemyShape;
					lEnemyShape.center = XMLoadFloat3(&backEnemy[i]->GetPosition());
					lEnemyShape.radius = backEnemy[i]->GetScale().z;

					if (Collision::CheckSphere2Sphere(pBullet, lEnemyShape)) {
						pb->OnCollision();
						backEnemy[i]->OnCollision();
						if (!backEnemy[i]->GetAlive()) {
							bEnePhase++;
						}
					}
				}
			}
			//障害物と自機弾の当たり判定
			Sphere obstacleShape;
			for (auto& ob : obstacles) {
				obstacleShape.center = XMLoadFloat3(&ob->GetPosition());
				obstacleShape.radius = ob->GetScale().x;

				if (Collision::CheckSphere2Sphere(pBullet, obstacleShape)) {
					pb->OnCollision();
				}
			}
		}
#pragma endregion
	}
	XMFLOAT3 playerPos = player->GetPosition();

	//プレイヤーのHPが0になったら画面切り替え
	if (playerLife <= 0 || playerPos.y <= -5) {
		player->CreateParticle();
		player->SetAlive(false);
		waitTimer++;
		if (waitTimer >= 30) {
			//シーン切り替え
			BaseScene* scene = new GameOver();
			this->sceneManager->SetNextScene(scene);
		}
	}
	if (fEnePhase >= 11 && lEnePhase >= 7 && rEnePhase >= 4 && bEnePhase >= 2) {
		//シーン切り替え
		BaseScene* scene = new GameClear();
		this->sceneManager->SetNextScene(scene);
	}
}