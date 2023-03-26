#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
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
	Sprite::LoadTexture(1, L"Resources/sosa_sinan.png");
	sprite.reset(Sprite::Create(1, { 0,0 }));
	Sprite::LoadTexture(2, L"Resources/HP.png");
	LifeSprite.reset(Sprite::Create(2, { 930,590 }));
	Sprite::LoadTexture(3, L"Resources/HP.png");
	LifeSprite2.reset(Sprite::Create(3, { 970,590 }));
	Sprite::LoadTexture(4, L"Resources/HP.png");
	LifeSprite3.reset(Sprite::Create(4, { 1010,590 }));
	Sprite::LoadTexture(5, L"Resources/HP.png");
	LifeSprite4.reset(Sprite::Create(5, { 1050,590 }));
	Sprite::LoadTexture(6, L"Resources/HP.png");
	LifeSprite5.reset(Sprite::Create(6, { 1090,590 }));
	Sprite::LoadTexture(7, L"Resources/HP.png");
	LifeSprite6.reset(Sprite::Create(7, { 1130,590 }));
	Sprite::LoadTexture(8, L"Resources/HP.png");
	LifeSprite7.reset(Sprite::Create(8, { 1170,590 }));
	Sprite::LoadTexture(9, L"Resources/HP.png");
	LifeSprite8.reset(Sprite::Create(9, { 1210,590 }));
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

	//当たり判定のインスタンス
	collisionMan = CollisionManager::GetInstance();
	//自機のオブジェクトセット+初期化
	player.reset(Player::Create(Model::CreateFromOBJ("octopus")));

	//床のオブジェクト生成
	floor.reset(TouchableObject::Create(Model::CreateFromOBJ("FloorBox")));
	floor->SetScale({ 20.f, 5.0f, 20.f });
	floor->SetPosition({ 0,-18.5f,0 });
	//パーティクのインスタンス
	particleMan = ParticleManager::GetInstance();
	particleMan->Initialize();
	particleMan->SetCamera(camera.get());

	//ドアの初期化
	for (int i = 0; i < 8; i++) {
		door[i] = new Door();
		door[i]->Initialize();

		//ドアの位置
		doorPos[i] = door[i]->GetPosition();
		doorPos[0] = { 8, 8, 50 };
		doorPos[1] = { -8,8,50 };
		doorPos[2] = { -50, 8, 8 };
		doorPos[3] = { -50,8,-8 };
		doorPos[4] = { 50, 8, 8 };
		doorPos[5] = { 50,8,-8 };
		doorPos[6] = { 8, 8, -50 };
		doorPos[7] = { -8,8,-50 };
		//ドアの向き
		doorRot[i] = door[i]->GetRotation();
		doorRot[2] = { 0, 90 ,0 };
		doorRot[3] = { 0, 90 ,0 };
		doorRot[4] = { 0, 90 ,0 };
		doorRot[5] = { 0, 90 ,0 };
	}

	//データ読み込み
	skyObj.reset(Object3d::Create());
	skyObj->SetModel(Model::CreateFromOBJ("skydome"));
}

void GamePlayScene::Finalize()
{
	//3Dオブジェクト解放
	for (int i = 0; i < 8; i++) {
		delete door[i];
	}
	//壁があったら削除する
	walls.remove_if([](std::unique_ptr<Wall>& wall) {
		return wall->GetAlive();
		});
}

void GamePlayScene::Update()
{
	
	Input* input = Input::GetInstance();

	// マウスを表示するかどうか(TRUEで表示、FALSEで非表示)
	ShowCursor(FALSE);
	// 座標の変更を反映
	SetCursorPos(960, 540);

	camera->SetFollowingTarget(player.get());

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
	//前敵が死んだら削除する
	frontEnemy.remove_if([](std::unique_ptr<FrontEnemy>& front) {
		return !front->GetAlive();
		});
	//左敵が死んだら削除する
	leftEnemy.remove_if([](std::unique_ptr<LeftEnemy>& left) {
		return !left->GetAlive();
		});
	//右敵が死んだら削除する
	rightEnemy.remove_if([](std::unique_ptr<RightEnemy>& right) {
		return !right->GetAlive();
		});
	//後ろ敵が死んだら削除する
	backEnemy.remove_if([](std::unique_ptr<BackEnemy>& back) {
		return !back->GetAlive();
		});

	LoadEnemyPopData();
	UpdataEnemyPopCommand();
	for (auto& front : frontEnemy) {
		front->Update();
	}
	for (auto& left : leftEnemy) {
		left->Update();
	}
	for (auto& right : rightEnemy) {
		right->Update();
	}
	for (auto& back : backEnemy) {
		back->Update();
	}

	//カメラの更新
	camera->Update();
	//床の更新
	floor->Update();

	skyObj->Update();
	//障害物のマップチップ読み込み用
	LoadObstaclePopData();
	UpdataObstaclePopCommand();
	//壁のマップチップ読み込み用
	LoadWallPopData();
	UpdataWallPopCommand();
	for (auto& wall : walls) {
		wall->Update();
	}
	DoorMove();

	//当たり判定
	collisionMan->CheckAllCollisions();
	CheckAllCollision();

	particleMan->Update();

}

//敵のPhaseに合わせて動かす
void GamePlayScene::DoorMove()
{
	//前ドア(右)
	if (doorPos[0].x >= 0) {
		doorPos[0].x -= 0.05f;
	}
	//前ドア(左)
	else if (doorPos[1].x >= -16 && fEnePhase >= 1) {
		doorPos[1].x -= 0.05f;
	}
	//左ドア(右)
	else if (doorPos[2].z >= 0 && fEnePhase >= 3) {
		doorPos[2].z -= 0.05;
	}
	//左ドア(左)
	else if (doorPos[3].z >= -16 && fEnePhase >= 9 && lEnePhase >= 3 && rEnePhase >= 2 && bEnePhase >= 1) {
		doorPos[3].z -= 0.05;
	}
	//右ドア(左)
	else if (doorPos[4].z >= 0 && fEnePhase >= 9 && lEnePhase >= 3 && rEnePhase >= 2 && bEnePhase >= 1) {
		doorPos[4].z -= 0.05;
	}
	//右ドア(右)
	else if (doorPos[5].z >= -16 && fEnePhase >= 5 && lEnePhase >= 1) {
		doorPos[5].z -= 0.05;
	}
	//後ろドア(左)
	else if (doorPos[6].x >= 0 && fEnePhase >= 7 && lEnePhase >= 2 && rEnePhase >= 1) {
		doorPos[6].x -= 0.05;
	}
	//後ろドア(右)
	else if (doorPos[7].x >= -16 && fEnePhase >= 9 && lEnePhase >= 3 && rEnePhase >= 2 && bEnePhase >= 1) {
		doorPos[7].x -= 0.05;
	}

	for (int i = 0; i < 8; i++) {
		door[i]->SetPosition(doorPos[i]);
		door[i]->SetRotation(doorRot[i]);
		door[i]->Update();
	}
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
	for (auto& front : frontEnemy) {
		front->Draw();
	}
	//左敵
	for (auto& left : leftEnemy) {
		left->Draw();
	}
	//右敵
	for (auto& right : rightEnemy) {
		right->Draw();
	}
	//後ろ敵
	for (auto& back : backEnemy) {
		back->Draw();
	}
	//障害物
	for (auto& obstacle : obstacles) {
		obstacle->Draw();
	}
	for (auto& wall : walls) {
		wall->Draw();
	}
	for (int i = 0; i < 8; i++) {
		door[i]->Draw();
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

void GamePlayScene::LoadEnemyPopData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	//ファイル内容を文字列ストリームにコピー
	enemyPopCom << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GamePlayScene::UpdataEnemyPopCommand()
{
	if (waitFlag) {
		//csv側のフェーズと敵フェーズが一致していたらWaitフラグをfalseにする
		if (fWaitPhase == fEnePhase && lWaitPhase == lEnePhase
			&& rWaitPhase == rEnePhase && bWaitPhase == bEnePhase) {
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
	while (getline(enemyPopCom, line)) {
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

		//FRONTPOPコマンド(前の敵用の座標コマンド)
		if (word.find("FRONTPOP") == 0) {
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
			newFront->SetPlayer(player.get());
			//障害物を登録する
			frontEnemy.push_back(std::move(newFront));
		}
		//LEFTPOPコマンド(左の敵用の座標コマンド)
		else if (word.find("LEFTPOP") == 0) {
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
			//コンストラクタ呼ぶ
			std::unique_ptr<LeftEnemy> newLeft = std::make_unique<LeftEnemy>();
			newLeft->Initialize(XMFLOAT3(x, y, z));
			newLeft->SetPlayer(player.get());
			//障害物を登録する
			leftEnemy.push_back(std::move(newLeft));
		}
		//RIGHTPOPコマンド(右の敵用の座標コマンド)
		else if (word.find("RIGHTPOP") == 0) {
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
			//コンストラクタ呼ぶ
			std::unique_ptr<RightEnemy> newRight = std::make_unique<RightEnemy>();
			newRight->Initialize(XMFLOAT3(x, y, z));
			newRight->SetPlayer(player.get());
			//障害物を登録する
			rightEnemy.push_back(std::move(newRight));
		}
		//BACKPOPコマンド(後ろの敵用の座標コマンド)
		else if (word.find("BACKPOP") == 0) {
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
			//コンストラクタ呼ぶ
			std::unique_ptr<BackEnemy> newBack = std::make_unique<BackEnemy>();
			newBack->Initialize(XMFLOAT3(x, y, z));
			newBack->SetPlayer(player.get());
			//障害物を登録する
			backEnemy.push_back(std::move(newBack));
		}
		//PHASEコマンド(敵の発生の順番)
		else if (word.find("PHASE") == 0) {
			getline(line_stream, word, ',');
			int frontPhase = atoi(word.c_str());

			getline(line_stream, word, ',');
			int leftPhase = atoi(word.c_str());

			getline(line_stream, word, ',');
			int rightPhase = atoi(word.c_str());

			getline(line_stream, word, ',');
			int backPhase = atoi(word.c_str());

			waitFlag = true;
			fWaitPhase = frontPhase;
			lWaitPhase = leftPhase;
			rWaitPhase = rightPhase;
			bWaitPhase = backPhase;
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
			//コンストラクタ呼ぶ
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

void GamePlayScene::LoadWallPopData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/wallPop.csv");
	assert(file.is_open());

	//ファイル内容を文字列ストリームにコピー
	wallPopCom << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GamePlayScene::UpdataWallPopCommand()
{
	//1行分の文字列を入れる変数
	std::string line;
	//コマンド実行ループ
	while (getline(wallPopCom, line)) {
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

			//x軸の回転
			getline(line_stream, word, ',');
			float rx = (float)std::atof(word.c_str());

			//y軸の回転
			getline(line_stream, word, ',');
			float ry = (float)std::atof(word.c_str());

			//z軸の回転
			getline(line_stream, word, ',');
			float rz = (float)std::atof(word.c_str());
			//敵を発生させる
			//コンストラクタ呼ぶよ
			std::unique_ptr<Wall> newWall = std::make_unique<Wall>();
			newWall->Initialize(XMFLOAT3(x, y, z), XMFLOAT3(rx, ry, rz));
			//障害物を登録する
			walls.push_back(std::move(newWall));
		}
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
			//壁と自機弾の当たり判定
			Sphere wallShape;
			for (auto& wall : walls) {
				wallShape.center = XMLoadFloat3(&wall->GetPosition());
				wallShape.radius = wall->GetScale().x;

				if (Collision::CheckSphere2Sphere(eBullet, wallShape)) {
					eb->OnCollision();
				}
			}
		}
	}
#pragma endregion
}

//左敵の弾の当たり判定
void GamePlayScene::LeftColl()
{
	for (auto& left : leftEnemy) {
		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = left->GetBullet();
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
				if (left->GetAlive()) {
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
			//壁と自機弾の当たり判定
			Sphere wallShape;
			for (auto& wall : walls) {
				wallShape.center = XMLoadFloat3(&wall->GetPosition());
				wallShape.radius = wall->GetScale().x;

				if (Collision::CheckSphere2Sphere(eBullet, wallShape)) {
					eb->OnCollision();
				}
			}
		}
	}
#pragma endregion
}


//右敵の弾の当たり判定
void GamePlayScene::RightColl()
{
	for (auto& right : rightEnemy) {
		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = right->GetBullet();
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
				if (right->GetAlive()) {
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
			//壁と自機弾の当たり判定
			Sphere wallShape;
			for (auto& wall : walls) {
				wallShape.center = XMLoadFloat3(&wall->GetPosition());
				wallShape.radius = wall->GetScale().x;

				if (Collision::CheckSphere2Sphere(eBullet, wallShape)) {
					eb->OnCollision();
				}
			}
		}

#pragma endregion
	}
}

//後ろ敵の弾の当たり判定
void GamePlayScene::BackColl()
{
	for (auto& back : backEnemy) {
		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = back->GetBullet();
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
				if (back->GetAlive()) {
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
			//壁と自機弾の当たり判定
			Sphere wallShape;
			for (auto& wall : walls) {
				wallShape.center = XMLoadFloat3(&wall->GetPosition());
				wallShape.radius = wall->GetScale().x;

				if (Collision::CheckSphere2Sphere(eBullet, wallShape)) {
					eb->OnCollision();
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
			for (auto& left : leftEnemy) {
				//左の敵
				if (left->GetAlive()) {
					Sphere lEnemyShape;
					lEnemyShape.center = XMLoadFloat3(&left->GetPosition());
					lEnemyShape.radius = left->GetScale().z;

					if (Collision::CheckSphere2Sphere(pBullet, lEnemyShape)) {
						pb->OnCollision();
						left->OnCollision();
						if (!left->GetAlive()) {
							lEnePhase++;
						}
					}
				}
			}
			for (auto& right : rightEnemy) {

				//右の敵
				if (right->GetAlive()) {
					Sphere lEnemyShape;
					lEnemyShape.center = XMLoadFloat3(&right->GetPosition());
					lEnemyShape.radius = right->GetScale().z;

					if (Collision::CheckSphere2Sphere(pBullet, lEnemyShape)) {
						pb->OnCollision();
						right->OnCollision();
						if (!right->GetAlive()) {
							rEnePhase++;
						}
					}
				}
			}
			for (auto& back : backEnemy) {

				//後ろの敵
				if (back->GetAlive()) {
					Sphere lEnemyShape;
					lEnemyShape.center = XMLoadFloat3(&back->GetPosition());
					lEnemyShape.radius = back->GetScale().z;

					if (Collision::CheckSphere2Sphere(pBullet, lEnemyShape)) {
						pb->OnCollision();
						back->OnCollision();
						if (!back->GetAlive()) {
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
			//壁と自機弾の当たり判定
			Sphere wallShape;
			for (auto& wall : walls) {
				wallShape.center = XMLoadFloat3(&wall->GetPosition());
				wallShape.radius = wall->GetScale().x;

				if (Collision::CheckSphere2Sphere(pBullet, wallShape)) {
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
	if (fEnePhase >= 11 && lEnePhase >= 6 && rEnePhase >= 5 && bEnePhase >= 2) {
		//シーン切り替え
		BaseScene* scene = new GameClear();
		this->sceneManager->SetNextScene(scene);
	}
}