#include "PlayerBullet.h"
#include <cassert>

PlayerBullet::PlayerBullet() :PlayerBullet(Model::LoadFromObj("sphere"))
{

}

void PlayerBullet::Initialize()
{

}

void PlayerBullet::Update()
{
	if (disappearTime < ++frameNum) {
		alive = false;
	}

	auto position = object->GetPosition();
	position.x += velocity.x;
	position.y += velocity.y;
	position.z += velocity.z;
	object->SetPosition(position);

	object->Update();
}

void PlayerBullet::Draw()
{
	if (alive) {
		object->Draw();
	}
}