#include "Player.h"
#include "Input.h"
#include "Shoot.h"
#include "DebugText.h"
#include "SphereCollider.h"

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


	if (!Object3d::Initialize()) {
		return false;
	}

	//コライダーの追加
	float radius = 0.6f;
	//半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));

	return true;
}

void Player::Update(Shoot* shootPos)
{
	Input* input = Input::GetInstance();
	XMFLOAT3 playerPos;
	// 移動後の座標を計算
	//移動
	/*if (input->PushKey(DIK_RIGHT)) { position.x += 0.15f; }
	if (input->PushKey(DIK_LEFT)) { position.x -= 0.15f; }
	if (input->PushKey(DIK_UP)) { position.y += 0.15f; }
	if (input->PushKey(DIK_DOWN)) { position.y -= 0.15f; }*/
	//回転
	if (input->PushKey(DIK_A)) { rotation.y -= 2.0f; }
	if (input->PushKey(DIK_D)) { rotation.y += 2.0f; }
	
	//移動ベクトルをY軸回りの角度で回転
	XMVECTOR move = { 0,0,0.1f,0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	move = XMVector3TransformNormal(move, matRot);

	if (input->PushKey(DIK_S)) {
		position.x -= move.m128_f32[0];
		position.z -= move.m128_f32[1];
		position.x -= move.m128_f32[2];
	}
	if (input->PushKey(DIK_W)) {
		position.x += move.m128_f32[0];
		position.z += move.m128_f32[1];
		position.x += move.m128_f32[2];
	}

	//移動範囲の制限
	if (position.x > 40) position.x = 40;
	if (position.x < -40) position.x = -40;

	// 座標の変更を反映
	playerObj->SetPosition(position);
	
	playerObj->Update();
	Object3d::Update();
}

void Player::OnCollision(const CollisionInfo& info)
{
	for (int i = 0; i < 4; i++) {

		enemy[i]->Hit();
	}
}

void Player::Draw()
{
	playerObj->Draw();
}
