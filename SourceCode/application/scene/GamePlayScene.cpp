#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
#include "SourceCode/application/enemy/EnemyBullet.h"
#include "Collision.h"
#include "ParticleManager.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "TouchableObject.h"
#include "SourceCode/engin/easing/Easing.h"
#include <fstream>
#include <cassert>

void GamePlayScene::Initialize(DirectXCommon* dxCommon)
{
	Sprite::LoadTexture(1, L"Resources/sosa_sinan.png");
	sprite.reset(Sprite::Create(1, { 0,0 }));

	Sprite::LoadTexture(16, L"Resources/alignment.png");
	alignment.reset(Sprite::Create(16, { 600,210 }));
	Sprite::LoadTexture(18, L"Resources/onAlignment.png");
	onAlignment.reset(Sprite::Create(18, { 600,210 }));

	Sprite::LoadTexture(17, L"Resources/damage.png");
	damage.reset(Sprite::Create(17, { 0,0 }));

	Sprite::LoadTexture(19, L"Resources/clear.png");
	clear.reset(Sprite::Create(19, { 0,-720 }));

	//phase
	phase.reset(new Phase());
	phase->Initialize();

	//ポストエフェクトの初期化
	postEffect.reset(new PostEffect());
	postEffect->CreateGraphicsPipelineState(L"Resources/shaders/PostEffectPS.hlsl");
	//シェーダーの挿入
	postEffect->Initialize();

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
	player->SetPosition({ 0, 30.f, 0 });
	//player->SetCollider(new SphereCollider());
	playerLife.reset(new PlayerLife());
	playerLife->Initialize();
	//床のオブジェクト生成
	floor.reset(TouchableObject::Create(Model::CreateFromOBJ("FloorBox")));
	floor->SetScale({ 20.f, 5.0f, 20.f });
	floor->SetPosition({ 0,-18.5f,0 });
	//パーティクのインスタンス
	particleMan = ParticleManager::GetInstance();
	particleMan->Initialize();
	particleMan->SetCamera(camera.get());

	//ドアの初期化
	door.reset(new Door());
	door->Initialize();
	//データ読み込み
	skyObj = Object3d::Create();
	skyObj->SetModel(Model::CreateFromOBJ("skydome"));
}

void GamePlayScene::Finalize()
{
	//解放
	camera.reset();
	skyObj.reset();
	door.reset();
	postEffect.reset();
	phase.reset();
	player.reset();
	playerLife.reset();
	//壁があったら削除する
	walls.remove_if([](std::unique_ptr<Wall>& wall) {
		return wall->GetAlive();
		});
	obstacles.remove_if([](std::unique_ptr<Obstacle>& obstacle) {
		return obstacle->GetAlive();
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

	//プレイヤーの更新
	player->Update();

	if (!phase->GetPhase()) {
		//フェーズ2
		if (fEnePhase >= 1) {
			phaseCount = 1;
		}
		//フェーズ3
		if (fEnePhase >= 3) {
			phaseCount = 2;
		}
		//フェーズ4
		if (fEnePhase >= 5 && lEnePhase >= 1) {
			phaseCount = 3;
		}
		//フェーズ5
		if (fEnePhase >= 7 && lEnePhase >= 2 && rEnePhase >= 1) {
			phaseCount = 4;
		}
		//フェーズ6
		if (fEnePhase >= 9 && lEnePhase >= 3 && rEnePhase >= 2 && bEnePhase >= 1) {
			phaseCount = 5;
		}
		phase->MovePhase(phaseCount);
	}
	if (phase->GetPhase()) {
		if (door) {
			door->DoorMove(phaseCount);
		}
		LoadEnemyPopData();
		UpdataEnemyPopCommand();
	}
	if (door) {
		door->Update();
	}

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
	for (auto& obstacle : obstacles) {
		obstacle->Update();
	}

	playerPos = player->GetPosition();
	if (!player->GetAlive()) {
		player->ScaleChange();
	}
	if (playerPos.y <= -10.0f) {
		player->ScaleChange();
	}
	//プレイヤーのHPが0になったらポストエフェクト
	if (!player->GetAlive() || playerPos.y <= -10.0f) {
		//中心に向かって暗くする
		endEfRadius = postEffect->GetRadius();
		endEfRadius -= 10.5f;
		if (endEfRadius <= 0.f) {
			endEfRadius = 0;
			endFlag = true;
		}
		postEffect->SetRadius(endEfRadius);
	}
	if (endFlag) {
		//シーン切り替え
		BaseScene* scene = new Tutorial();
		this->sceneManager->SetNextScene(scene);
	}

	if (fEnePhase >= 11 && lEnePhase >= 5 && rEnePhase >= 4 && bEnePhase >= 3) {
		clearFlag = true;
	}
	if (clearFlag) {
		clearPos = clear->GetPosition();
		if (clearPos.y <= 0) {
			if (easFrame < 1.0f) {
				easFrame += 0.009f;
			}
			clearPos.y = Ease(In, Elastic, easFrame, -720.0f, 15.0f);

			clear->SetPosition(clearPos);
		}
		else {
			clearTime++;
		}
		
		if (clearTime >= 120) {
		endEfRadius = postEffect->GetRadius();
		endEfRadius -= 15.5f;
		if (endEfRadius <= 0.f) {
			endEfRadius = 0;
			clearTFlag = true;
		}
		postEffect->SetRadius(endEfRadius);

		}
	}
	if (clearTFlag) {
		//シーン切り替え
		BaseScene* scene = new TitleScene();
		this->sceneManager->SetNextScene(scene);
	}

	player->SetPosition(playerPos);
	//カメラの更新
	camera->Update();
	//床の更新
	floor->Update();

	life = player->GetLife();
	playerLife->Update(life);
	player->SetLife(life);
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

	//当たり判定
	CheckAllCollision();
	collisionMan->CheckAllCollisions();

	particleMan->Update();

}

void GamePlayScene::Draw(DirectXCommon* dxCommon)
{
	//描画前処理
	//dxCommon->PreDraw();
	//スプライト描画
#pragma region 背景スプライト描画
	postEffect->PreDrawScene(dxCommon->GetCmdList());

#pragma endregion

#pragma endregion

	//3Dオブジェクト描画前処理
	Object3d::PreDraw();
	//奥にあるほど先に書く
	skyObj->Draw();
	floor->Draw();
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
	for (auto& wall : walls) {
		wall->Draw();
	}
	if (door) {
		door->Draw();
	}
	player->Draw();
	//障害物
	for (auto& obstacle : obstacles) {
		obstacle->Draw();
	}

	particleMan->Draw();

	Object3d::PostDraw();

	postEffect->PostDrawScene(dxCommon->GetCmdList());

	//描画前処理
	dxCommon->PreDraw();
	postEffect->Draw(dxCommon->GetCmdList());
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCmdList());

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite->Draw();
	playerLife->Draw();
	alignment->Draw();
	if (damageFlag1 || damageFlag2 || damageFlag3 || damageFlag4) {
		damage->Draw();
	}
	//フェーズ変更時のスプライト
	phase->Draw(phaseCount);
	//クリア時表示する
	if (clearFlag) {
		clear->Draw();
	}
	// スプライト描画後処理

	Sprite::PostDraw();
	//描画後処理
	dxCommon->PostDraw();

#pragma region 前景スプライト描画
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
			phase->SetPhase(false);
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
		/*else if (word.find("TIME") == 0) {
			getline(line_stream, word, ',');
			int32_t frontPhase = atoi(word.c_str());


		}*/
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
			//eb->SetCollider()
			Sphere eBullet;

			if (eb->GetAlive()) {
				eBullet.center = XMLoadFloat3(&eb->GetPosition());
				eBullet.radius = eb->GetScale().x;
				Sphere playerShape;
				playerShape.center = XMLoadFloat3(&player->GetPosition());
				playerShape.radius = player->GetScale().x;

				if (Collision::CheckSphere2Sphere(eBullet, playerShape)) {

					eb->OnCollision();
					player->OnCollision();
					if (!damageFlag1) {
						damageFlag1 = true;
					}
				}
				if (damageFlag1) {
					color = damage->GetColor();
					color.w -= 0.05f;
					if (color.w <= 0.0f) {
						color.w = 1.0f;
						damageFlag1 = false;
					}
					damage->SetColor(color);
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
					player->OnCollision();
					if (!damageFlag2) {
						damageFlag2 = true;
					}
				}
				if (damageFlag2) {
					color = damage->GetColor();
					color.w -= 0.05f;
					if (color.w <= 0.0f) {
						color.w = 1.0f;
						damageFlag2 = false;
					}
					damage->SetColor(color);
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
					player->OnCollision();
					if (!damageFlag3) {
						damageFlag3 = true;
					}
				}
				if (damageFlag3) {
					color = damage->GetColor();
					color.w -= 0.05f;
					if (color.w <= 0.0f) {
						color.w = 1.0f;
						damageFlag3 = false;
					}
					damage->SetColor(color);
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
					player->OnCollision();
					if (!damageFlag4) {
						damageFlag4 = true;
					}
				}
				if (damageFlag4) {
					color = damage->GetColor();
					color.w -= 0.05f;
					if (color.w <= 0.0f) {
						color.w = 1.0f;
						damageFlag4 = false;
					}
					damage->SetColor(color);
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
}