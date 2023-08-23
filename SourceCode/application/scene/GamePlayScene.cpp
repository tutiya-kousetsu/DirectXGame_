#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
#include "EnemyBullet.h"
#include "Collision.h"
#include "ParticleManager.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include "MeshCollider.h"
#include "TouchableObject.h"
#include "Easing.h"
#include <fstream>
#include <cassert>

void GamePlayScene::Initialize(DirectXCommon* dxCommon)
{
	//音の読み込み
	Audio* audio = Audio::GetInstance();
	audio->SoundLoadWave("stone.wav");
	//痺れた時のSE
	audio->SoundLoadWave("numb.wav");
	audio->SoundLoadWave("gamePlay.wav");

	//スプライト読み込み
	Sprite::LoadTexture(1, L"Resources/sosa_sinan.png");
	sprite.reset(Sprite::Create(1, { 640,360 }));

	Sprite::LoadTexture(16, L"Resources/alignment.png");
	alignment.reset(Sprite::Create(16, { 635,235 }));
	Sprite::LoadTexture(18, L"Resources/onAlignment.png");
	onAlignment.reset(Sprite::Create(18, { 640,240 }));

	Sprite::LoadTexture(17, L"Resources/damage.png");
	damage.reset(Sprite::Create(17, { 640,360 }));

	Sprite::LoadTexture(19, L"Resources/clear/upFrame.png");
	upClear.reset(Sprite::Create(19, { 640,-56.5f }));
	Sprite::LoadTexture(29, L"Resources/clear/string.png");
	clear.reset(Sprite::Create(29, { 640,360 }));
	clear->SetSize({ 3280, 2493 });
	Sprite::LoadTexture(30, L"Resources/clear/downFrame.png");
	downClear.reset(Sprite::Create(30, { 640,776.5f }));

	Sprite::LoadTexture(31, L"Resources/standby.png");
	standby.reset(Sprite::Create(31, { 640,360 }));
	Sprite::LoadTexture(32, L"Resources/standby2.png");
	standby2.reset(Sprite::Create(32, { 640,360 }));

	//phase
	phase.reset(new Phase());
	phase->Initialize();
	//矢印のスプライト
	arrow.reset(new Arrow());
	arrow->Initialize();

	//ポストエフェクトの初期化
	postEffect.reset(new PostEffect());
	postEffect->CreateGraphicsPipelineState(L"Resources/shaders/PostEffectPS.hlsl");
	//シェーダーの挿入
	postEffect->Initialize();

	//カメラの初期化
	camera.reset(new FollowingCamera());
	//カメラを3Dオブジェットにセット
	Object3d::SetCamera(camera.get());
	debugCam.reset(new DebugCamera(WinApp::window_width, WinApp::window_height));

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
	player->SetPosition({ 0.f, 30.f, 0.f });
	player->SetScale({ 0.f,0.f,0.f });
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


	//壁のマップチップ読み込み用
	LoadWallPopData();
	LoadObstaclePopData();
	LoadEnemyPopData();
}

void GamePlayScene::Finalize()
{
	//解放
	camera.reset();
	skyObj.reset();
	door.reset();
	postEffect.reset();
	phase.reset();
	arrow.reset();
	player.reset();
	playerLife.reset();
	CollisionManager::GetInstance()->RemoveCollider(collider);
	delete collider;
	delete input;
	//壁があったら削除する
	walls.remove_if([](std::unique_ptr<Wall>& wall) {
		return wall->GetAlive();
		});
	obstacles.remove_if([](std::unique_ptr<Obstacle>& obstacle) {
		return obstacle->GetAlive();
		});
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

}

void GamePlayScene::Update()
{
	Input* input = Input::GetInstance();
	Audio* audio = Audio::GetInstance();
	//ゲームプレイが始まったら音を消す
	audio->SoundStop("water.wav");
	playerPos = player->GetPosition();

	//メンバ関数のポインタに入ってる関数を呼び出す
	(this->*gProgress[static_cast<size_t>(gamePhase)])();
	//障害物のマップチップ読み込み用
	UpdataObstaclePopCommand();
	for (auto& obstacle : obstacles) {
		obstacle->Update();
	}
	// マウスを表示するかどうか(TRUEで表示、FALSEで非表示)
	ShowCursor(FALSE);
	// 座標の変更を反映
	SetCursorPos(960, 540);

	//カメラにプレイヤーを固定させる
	camera->SetFollowingTarget(player.get());

	// マウスの入力を取得
	if (landTime >= 230 && !numbFlag && !popFlag) {
		player->Mouse();
	}

	//痺れたらプレイヤーの動きを止める
	if (numbFlag) {
		player->StopUpdate();
		player->Numb(numbFlag);
	}

	if (landTime >= 230 && !numbFlag && aliveFlag && !popFlag) {
		//プレイヤーの更新
		player->Update();
	}
	//フェーズフラグが立つごとにドアを動かす、敵のcsvの更新をする
	if (phaseFlag && !popFlag) {
		if (door) {
			door->DoorMove(phaseCount);
		}
		UpdataEnemyPopCommand();

		//敵の更新処理
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
	}
	if (door) {
		door->Update();
	}

	if (input->TriggerKey(DIK_Q)) {
		if (!popFlag) {
			popFlag = true;
			standbyFlag = true;
		}
	}

	if (popFlag) {
		if (input->TriggerKey(DIK_RIGHT) && standbyFlag) {
			standby2Flag = true;
			standbyFlag = false;
		}
		if (input->TriggerKey(DIK_LEFT) && standby2Flag) {
			standby2Flag = false;
			standbyFlag = true;
		}
		if (input->TriggerKey(DIK_SPACE)) {
			if (standbyFlag && !standby2Flag) {
				audio->SoundStop("gamePlay.wav");
				//シーン切り替え
				BaseScene* scene = new TitleScene();
				this->sceneManager->SetNextScene(scene);
			}
			if (!standbyFlag && standby2Flag) {
				popFlag = false;
			}
		}
		for (auto& front : frontEnemy) {
			front->StopUpdate();
		}
		for (auto& left : leftEnemy) {
			left->StopUpdate();
		}
		for (auto& right : rightEnemy) {
			right->StopUpdate();
		}
		for (auto& back : backEnemy) {
			back->StopUpdate();
		}
		player->StopUpdate();
	}

	//クリアしたときの関数
	Clear();

	//ゲームオーバーになった時
	Failed();

	player->SetPosition(playerPos);
	//カメラの更新
	camera->Update();
	debugCam->Update();

	//床の更新
	floor->Update();

	life = player->GetLife();
	player->SetLife(life);
	skyObj->Update();
	UpdataWallPopCommand();
	for (auto& wall : walls) {
		wall->Update();
	}

	//当たり判定
	CheckAllCollision();
	collisionMan->CheckAllCollisions();
	particleMan->Update();
}

void (GamePlayScene::* GamePlayScene::gProgress[])() = {
	&GamePlayScene::Air,
	&GamePlayScene::Landing,
	&GamePlayScene::GameStart
};

//スタート時空中にいるときの関数
void GamePlayScene::Air()
{
	sceneTime++;
	//追従カメラから普通のカメラに変更
	nowCamera = debugCam.get();
	Object3d::SetCamera(nowCamera);
	nowCamera->SetEye({ playerPos.x, 2.f, 20.f });
	nowCamera->SetTarget(playerPos);

	if (inFrame < 1.0f) {
		inFrame += 0.01f;
		//プレイヤーを大きくする関数
		player->ScaleLarge();
	}
	playerPos.y = Ease(InOut, Cubic, inFrame, 30.f, -1.83f);
	playerScale = player->GetScale();
	//プレイヤーの大きさが一定以上大きくなったら次のgamePhaseに移行する
	if (playerScale.x >= fixedScale && playerScale.y >= fixedScale && playerScale.z >= fixedScale) {
		gamePhase = GamePhase::Landing;
		landFlag = true;
	}
	player->SetPosition(playerPos);
	player->StopUpdate();
}

//着地した時の関数
void GamePlayScene::Landing()
{
	//着地してからタイマーを進める
	landTime++;
	//時間が10～210の間だけ岩が動いてるときの音を流す
	if (landTime >= 10 && landTime <= 210) {
		audio->SoundPlayWave("stone.wav", false);
	}
	//時間が210になったら音を止める
	if (landTime == 210) {
		// サウンド停止
		audio->SoundStop("stone.wav");
		audio->SoundPlayWave("gamePlay.wav", true);
	}
	//230より小さい間岩を押し上げる関数を呼ぶ
	if (landTime <= 230) {
		for (auto& ob : obstacles) {
			ob->UpMove(landFlag);
		}
	}
	//プレイヤーを動かせないようにするための関数
	player->StopUpdate();
	//次のゲームフェーズへ移行する
	if (landTime >= 230) {
		gamePhase = GamePhase::GameStart;
	}
}

//着地して岩が上がりきった時、フェーズ1から始める関数
void GamePlayScene::GameStart()
{
	//カメラの切り替え(自機の追従カメラに)
	nowCamera = camera.get();
	Object3d::SetCamera(nowCamera);
	phaseFlag = phase->GetPhase();
	if (!phaseFlag) {
		//フェーズ2に移行するための条件
		if (fEneCount >= 1) {
			phaseCountFlag = true;
			//flag = true;
			// サウンド再生
			audio->SoundPlayWave("stone.wav", true);
			phaseCount = 1;
		}
		//フェーズ3に移行するための条件
		if (fEneCount >= 2 && lEneCount >= 1) {
			phaseCount = 2;
		}
		//フェーズ4に移行するための条件
		if (fEneCount >= 4 && lEneCount >= 2) {
			phaseCount = 3;
		}
		//フェーズ5に移行するための条件
		if (fEneCount >= 6 && lEneCount >= 3 && rEneCount >= 1) {
			phaseCount = 4;
		}
		//フェーズ6に移行するための条件
		if (fEneCount >= 8 && lEneCount >= 4 && rEneCount >= 2 && bEneCount >= 1) {
			phaseCount = 5;
		}
		//フェーズのスプライトの移動関数
		phase->MovePhase(phaseCount);
	}
	//フェーズフラグが立ったら
	if (phaseCountFlag) {
		downTime++;
		//カメラの切り替え
		nowCamera = debugCam.get();
		Object3d::SetCamera(nowCamera);
		nowCamera->SetEye({ 0, 50.f, -1.f });
		nowCamera->SetTarget({ 0, -90, 0 });
		//タイムが230行ったら岩を下に下げるのを止める
		if (downTime <= 230) {
			for (auto& ob : obstacles) {
				ob->DownMove(phaseCountFlag);
			}
		}
	}
	//230超えたらあげるフラグを立てて岩を上に上げる
	if (downTime >= 230) {
		upFlag = true;
		for (auto& ob : obstacles) {
			ob->UpMove(upFlag);
		}
	}
	//フラグとタイムの初期化
	if (downTime >= 460) {
		phaseCountFlag = false;
		upFlag = false;
		downTime = 0;
		// サウンド停止
		audio->SoundStop("stone.wav");
	}
	//自機が弾を打てなくする
	if (downTime <= 460) {
		player->SetPhaseFlag(phaseCountFlag);

	}

	//カメラの切り替え
	if (!phaseCountFlag) {
		nowCamera = camera.get();
		Object3d::SetCamera(nowCamera);
	}
}

void GamePlayScene::Clear()
{
	float clearMove = 200;

	Audio* audio = Audio::GetInstance();
	//クリア条件
	if (fEneCount >= 10 && lEneCount >= 6 && rEneCount >= 4 && bEneCount >= 3) {
		clearFlag = true;
		audio->SoundStop("gamePlay.wav");
	}
	//クリアフラグが立ったらif文通す
	if (clearFlag) {
		//クリアしたらイージングでクリアのスプライト動かす
		upPos = upClear->GetPosition();
		if (upPos.y <= 56.5) {
			if (upFrame < 1.0f) {
				upFrame += 0.01f;
			}
			upPos.y = Ease(In, Cubic, upFrame, upPos.y, 56.5f);
			upClear->SetPosition(upPos);
		}
		//枠ができったらフラグを立てる
		if (upPos.y == 56.5f) {
			stringFlag = true;
		}
		//下から上がってくるスプライト
		downPos = downClear->GetPosition();
		if (downPos.y >= 663.5f) {
			if (downFrame < 1.0f) {
				downFrame += 0.01f;
			}
			downPos.y = Ease(In, Cubic, downFrame, downPos.y, 663.5f);
			downClear->SetPosition(downPos);
		}
		//フラグが立ったら文字の部分を元の大きさまで小さくする
		if (stringFlag) {
			clearSize = clear->GetSize();
			if (clearSize.x >= 1380 && clearSize.y >= 593) {
				clearSize.x -= clearMove;
				clearSize.y -= clearMove;
			}
			//クリアスプライトが元の大きさに戻ったらタイマースタートさせる
			else {
				clearTime++;
			}
			clear->SetSize(clearSize);
		}

		//タイマーが120経ったらポストエフェクトで中心に向かって暗くする
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
	//全部暗くなったらタイトルに戻す
	if (clearTFlag) {
		//シーン切り替え
		BaseScene* scene = new TitleScene();
		this->sceneManager->SetNextScene(scene);
	}
}

void GamePlayScene::Failed()
{
	playerPos = player->GetPosition();
	aliveFlag = player->GetAlive();

	//自機がステージから落ちたら小さくする
	if (playerPos.y <= -10.0f) {
		landingFlag = true;
	}
	if (!aliveFlag) {
		player->StopUpdate();
	}
	//プレイヤーのHPが0になったらポストエフェクト
	if (!aliveFlag || landingFlag) {
		player->ScaleSmall();
		//中心に向かってポストエフェクトで暗くする
		endEfRadius = postEffect->GetRadius();
		endEfRadius -= 10.5f;
		if (endEfRadius <= 0.f) {
			endEfRadius = 0;
			endFlag = true;
		}
		postEffect->SetRadius(endEfRadius);
	}
	if (endFlag && !clearFlag) {
		audio->SoundStop("gamePlay.wav");
		audio->SoundStop("stone.wav");
		//シーン切り替え
		BaseScene* scene = new Tutorial();
		this->sceneManager->SetNextScene(scene);
	}
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

	if (downTime <= 0) {
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
	}
	if (sceneTime >= 10) {
		for (auto& wall : walls) {
			wall->Draw();
		}
		door->Draw();
	}
	player->Draw(numbFlag);
	//障害物
	if (landTime >= 10) {
		for (auto& obstacle : obstacles) {
			obstacle->Draw();
		}
		//フラグが立ったら半透明の岩を後に描画する
		for (auto& obstacle : obstacles) {
			if (rayFlag) {
				obstacle->Draw();
			}
		}
	}
	if (downTime <= 0) {
		particleMan->Draw();
	}
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

	//ゲームオーバーとクリアのフラグが経ってない時に描画してる
	if (!clearFlag && aliveFlag) {
		sprite->Draw();
		playerLife->Draw(life);
		if (landTime >= 230 && downTime <= 0) {
			alignment->Draw();
		}

		if (damageFlag1 || damageFlag2 || damageFlag3 || damageFlag4) {
			if (!phaseCountFlag) {
				damage->Draw();
			}
		}
		//フェーズ変更時のスプライト
		phase->Draw(phaseCount);
		if (phaseCountFlag) {
			arrow->Draw(phaseCount);
		}
	}
	//クリア時表示する
	if (clearFlag) {
		if (stringFlag) {
			clear->Draw();
		}
		upClear->Draw();
		downClear->Draw();
	}
	if (popFlag && standbyFlag) {
		standby->Draw();
	}
	if (popFlag && standby2Flag) {
		standby2->Draw();
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
	file.open("Resources/csv/enemyPop.csv");
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
		if (fWaitPhase == fEneCount && lWaitPhase == lEneCount
			&& rWaitPhase == rEneCount && bWaitPhase == bEneCount) {
			waitFlag = false;
			phase->SetPhase(false);
		}
		else {
			//一致していなかったらreturnで返す
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
			//コンストラクタ呼ぶ
			std::unique_ptr<LeftEnemy> newLeft = std::make_unique<LeftEnemy>();
			newLeft->Initialize(XMFLOAT3(x, y, z), XMFLOAT3(rx, ry, rz));
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
	file.open("Resources/csv/ObstaclePop.csv");
	assert(file.is_open());

	//ファイル内容を文字列ストリームにコピー
	obstaclePopCom << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GamePlayScene::UpdataObstaclePopCommand()
{
	if (phFlag) {
		//csv側のフェーズと敵フェーズが一致していたらWaitフラグをfalseにする
		if (obWaitPhase == phaseCount) {
			phFlag = false;
			//phase->SetPhase(false);
		}
		//一致していなかったらreturnで返す
		else {
			return;
		}
	}

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
		else if (word.find("PHASE") == 0) {
			getline(line_stream, word, ',');
			int obPhase = atoi(word.c_str());

			phFlag = true;
			obWaitPhase = obPhase;
			break;
		}
	}
}

void GamePlayScene::LoadWallPopData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/csv/wallPop.csv");
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
					player->OnCollision(1);
					if (!damageFlag1) {
						damageFlag1 = true;
					}
				}
				//フラグが立ったらダメージ演出のスプライトを描画してだんだん薄くしていく
				if (damageFlag1 && !phaseCountFlag) {
					color1 = damage->GetColor();
					color1.w -= 0.05f;
					if (color1.w <= 0.0f) {
						color1.w = 1.0f;
						damageFlag1 = false;
					}
					damage->SetColor(color1);
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
					//自機のHPが1減る
					player->OnCollision(1);
					if (!damageFlag2 && !phaseCountFlag) {
						damageFlag2 = true;
					}
				}
				//フラグが立ったらダメージ演出のスプライトを描画してだんだん薄くしていく
				if (damageFlag2 && !phaseCountFlag) {
					color2 = damage->GetColor();
					color2.w -= 0.05f;
					if (color2.w <= 0.0f) {
						color2.w = 1.0f;
						damageFlag2 = false;
					}
					damage->SetColor(color2);
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
					//自機のHPが2減る
					player->OnCollision(2);
					if (!damageFlag3 && !phaseCountFlag) {
						damageFlag3 = true;
					}
				}
				//フラグが立ったらダメージ演出のスプライトを描画してだんだん薄くしていく
				if (damageFlag3 && !phaseCountFlag) {
					color3 = damage->GetColor();
					color3.w -= 0.05f;
					if (color3.w <= 0.0f) {
						color3.w = 1.0f;
						damageFlag3 = false;
					}
					damage->SetColor(color3);
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
					audio->SoundPlayWave("numb.wav", false);
					eb->OnCollision();
					player->OnCollision(1);
					if (!numbFlag && life > 0) {
						numbFlag = true;
					}
					if (!damageFlag4 && !phaseCountFlag) {
						damageFlag4 = true;
					}
				}
				//フラグが立ったらダメージ演出のスプライトを描画してだんだん薄くしていく
				if (damageFlag4 && !phaseCountFlag) {
					color4 = damage->GetColor();
					color4.w -= 0.05f;
					if (color4.w <= 0.0f) {
						color4.w = 1.0f;
						damageFlag4 = false;
					}
					damage->SetColor(color4);
				}

				//フラグが立ったらタイムを進める
				if (numbFlag) {
					numbTime++;
				}
				//Timeが1秒経ったら初期化
				if (numbTime >= 5) {
					numbFlag = false;
					numbTime = 0;
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
	audio = Audio::GetInstance();
	FrontColl();
	LeftColl();
	RightColl();
	BackColl();
	Input* input = Input::GetInstance();
	//レイの当たり判定(当たったら岩を透明にする)
	Sphere obShape;
	//痺れた時にレイだけ動かないようにする
	if (landTime >= 230 && !numbFlag && !standbyFlag) {
		Input::MouseMove mouseMove = input->GetMouseMove();
		float dy = mouseMove.lX * scaleY;
		angleY = -dy * XM_PI;
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

		XMFLOAT3 target1 = camera->GetTarget();
		camera->SetEye({ target1.x + vTargetEye.m128_f32[0], target1.y + vTargetEye.m128_f32[1], target1.z + vTargetEye.m128_f32[2] });
		camera->SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });

		// 注視点からずらした位置に視点座標を決定
		XMFLOAT3 target2 = camera->GetTarget();
		XMFLOAT3 eye = camera->GetEye();
		XMFLOAT3 fTargetEye = { 0.0f, 0.0f, 0.0f };
		//正規化
		fTargetEye.x = eye.x - target2.x;
		fTargetEye.y = eye.y - target2.y;
		fTargetEye.z = eye.z - target2.z;

		Ray ray;
		ray.start = XMLoadFloat3(&camera->GetTarget());
		ray.dir = XMLoadFloat3(&fTargetEye);
		for (auto& ob : obstacles) {
			obShape.center = XMLoadFloat3(&ob->GetPosition());
			obShape.radius = ob->GetScale().x;
			if (Collision::CheckRay2Sphere(ray, obShape)) {
				rayFlag = true;
				if (!phaseCountFlag) {
					ob->OnCollision(rayFlag);
				}
			}
			else {
				ob->OnCollision(!rayFlag);
			}
		}
	}
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
							fEneCount++;
							wait--;
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
							lEneCount++;
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
							rEneCount++;
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
							bEneCount++;
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
					ob->OnCollision();

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