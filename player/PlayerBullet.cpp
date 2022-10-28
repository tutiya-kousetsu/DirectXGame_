#include "PlayerBullet.h"
#include <cassert>

PlayerBullet::PlayerBullet() :PlayerBullet(Model::LoadFromObj("sphere"))
{
	//ƒf[ƒ^“Ç‚Ýž‚Ý
	object->SetScale({ 1.0f, 1.0f, 1.0f });
}

void PlayerBullet::Initialize()
{

}

void PlayerBullet::Update()
{
	if (disappearTime < ++frameNum) {
		alive = false;
	}

	object->Update();
}

void PlayerBullet::Draw()
{
	if (alive) {
		object->Draw();
	}
}