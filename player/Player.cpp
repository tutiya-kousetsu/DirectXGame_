#include "Player.h"
#include "Input.h"
#include "Shoot.h"
#include "DebugText.h"
#include "Enemy.h"
using namespace DirectX;

Player* Player::Create(Model* model)
{
	//3Dオブジェクトのインスタンス生成
	Player* instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	//モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

Player::Player()
{
}

Player::~Player()
{
	delete(playerObj);
	delete(playerModel);
}

bool Player::Initialize()
{
	////メンバ変数にコピー
	//this->input = input;

	//データ読み込み
	playerModel = Model::LoadFromObj("PlayerRed");
	playerObj = Object3d::Create();
	playerObj->SetModel((playerModel));
	playerObj->SetScale({ 0.75f, 0.75f, 0.75f });

	//データ読み込み
	shootModel = Model::LoadFromObj("sphere");
	shootObj = Object3d::Create();
	shootObj->SetModel(shootModel);
	shootObj->SetScale({ 0.5f, 0.5f, 0.5f });

}

//移動処理
void Player::Move() {
	Input* input = Input::GetInstance();
	//回転
	XMFLOAT3 playerRot = playerObj->GetRotation();
	playerRot = { 0,90,0 };

	if (input->PushKey(DIK_RIGHT)) { rotation.y -= 2.0f; }
	if (input->PushKey(DIK_LEFT)) { rotation.y += 2.0f; }
	playerObj->SetRotation(rotation);
	//移動ベクトルをY軸回りの角度で回転
	XMVECTOR move = { 0,0,0.5f,0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	move = XMVector3TransformNormal(move, matRot);

	if (input->PushKey(DIK_A)) {
		position.x -= move.m128_f32[0];
		position.z -= move.m128_f32[1];
		position.x -= move.m128_f32[2];
	}
	else if (input->PushKey(DIK_D)) {
		position.x += move.m128_f32[0];
		position.z += move.m128_f32[1];
		position.x += move.m128_f32[2];
	}
	if (input->PushKey(DIK_S)) {
		position.y -= move.m128_f32[0];
		position.z -= move.m128_f32[1];
		position.y -= move.m128_f32[2];
	}
	else if (input->PushKey(DIK_W)) {
		position.y += move.m128_f32[0];
		position.z += move.m128_f32[1];
		position.y += move.m128_f32[2];
	}
	//移動範囲の制限
	if (position.x > 40) position.x = 40;
	if (position.x < -40) position.x = -40;

	// 座標の変更を反映
	playerObj->SetPosition(position);
}

void Player::Update()
{
	Move();
	playerObj->Update();

}

void Player::Draw()
{
	playerObj->Draw();
}
