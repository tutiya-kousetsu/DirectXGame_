#include "Obstacle.h"
#include "BaseCollider.h"
#include "CollisionManager.h"
#include "Easing.h"
#include <fstream>

Obstacle::~Obstacle()
{
	if (collider) {
		//コリジョンマネージャから登録を解除
		CollisionManager::GetInstance()->RemoveCollider(collider);
		delete collider;
	}
}

bool Obstacle::Initialize(XMFLOAT3 position)
{
	object.reset(new TouchableObject());
	object->Initialize(Model::CreateFromOBJ("stone"));
	object->SetScale({ 3.4f, 3.21f, 3.4f });
	shake.reset(new Shake());
	this->position = position;
	return true;

}

void Obstacle::Update()
{
	object->Update();
}

void Obstacle::Draw()
{
	object->Draw();
}

void Obstacle::UpMove(bool landF)
{
	float move = 0.1f;
	//着地したら
	if (landF) {
			//イージングで押し上げる
			if (easFrame < 1.0f) {
				easFrame += 0.0001f;
			}
			if (position.y <= -0.1f) {
				position.y = Ease(Out, Cubic, easFrame, position.y, 0);
			}
			//シェイクのフラグを立てる
			shake->SetShakeStart(true);
			//ポジションのy座標が-0.1まで行ったらフラグをfalseにする
			if (position.y >= -0.1f) {
				shake->SetShakeStart(false);
				easFrame = 0;
			}
	}
	//シェイクの最大値最小値などを入れる
	shake->ShakePos(shakePos.x, 1, -1, 10, 20);
	shake->ShakePos(shakePos.z, 1, -1, 10, 20);
	//フラグがfalseだったらシェイクをともる
	if (!shake->GetShakeStart()) {
		shakePos = { 0.0f, 0.0f, 0.0f };
	}
	//ポジションにシェイクの動きを足す
	position.x += shakePos.x;
	position.z += shakePos.z;

	SetPosition(position);
}

void Obstacle::DownMove(bool flag)
{
	float move = 0.1f;
	//着地したら
	if (flag) {
			//イージングで押し上げる
			if (downFrame < 1.0f) {
				downFrame += 0.0001f;
			}
		if (position.y >= -9.9f) {
			position.y = Ease(Out, Quad, downFrame, position.y, -10.0f);
		}
			//シェイクのフラグを立てる
			shake->SetShakeStart(true);
			//ポジションのy座標が-0.1まで行ったらフラグをfalseにする
			if (position.y <= -9.9f) {
				shake->SetShakeStart(false);
				downFrame = 0;
			}
	}
	//シェイクの最大値最小値などを入れる
	shake->ShakePos(shakePos.x, 1, -1, 10, 20);
	shake->ShakePos(shakePos.z, 1, -1, 10, 20);
	//フラグがfalseだったらシェイクをともる
	if (!shake->GetShakeStart()) {
		shakePos = { 0.0f, 0.0f, 0.0f };
	}
	//ポジションにシェイクの動きを足す
	position.x += shakePos.x;
	position.z += shakePos.z;

	SetPosition(position);
}

void Obstacle::OnCollision(bool flag)
{

	if (flag) {
		color = object->GetColor();
		color.w = 0.45f;
		object->SetColor({ 1.0f, 1.0f, 1.0f, color.w });
	}
	if (!flag) {
		object->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	}
}