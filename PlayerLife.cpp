#include "PlayerLife.h"

void PlayerLife::Initialize()
{
	Sprite::LoadTexture(2, L"Resources/HP.png");
	lifeSprite[0].reset(Sprite::Create(2, { 930,595 }));
	Sprite::LoadTexture(3, L"Resources/HP.png");
	lifeSprite[1].reset(Sprite::Create(3, { 970,595 }));
	Sprite::LoadTexture(4, L"Resources/HP.png");
	lifeSprite[2].reset(Sprite::Create(4, { 1010,595 }));
	Sprite::LoadTexture(5, L"Resources/HP.png");
	lifeSprite[3].reset(Sprite::Create(5, { 1050,595 }));
	Sprite::LoadTexture(6, L"Resources/HP.png");
	lifeSprite[4].reset(Sprite::Create(6, { 1090,595 }));
	Sprite::LoadTexture(7, L"Resources/HP.png");
	lifeSprite[5].reset(Sprite::Create(7, { 1130,595 }));
	Sprite::LoadTexture(8, L"Resources/HP.png");
	lifeSprite[6].reset(Sprite::Create(8, { 1170,595 }));
	Sprite::LoadTexture(9, L"Resources/HP.png");
	lifeSprite[7].reset(Sprite::Create(9, { 1210,595 }));

}

void PlayerLife::Update(int playerLife)
{
	life = playerLife;
}

void PlayerLife::Draw()
{
	if (life >= 7) { lifeSprite[7]->Draw(); }
	if (life >= 6) { lifeSprite[6]->Draw(); }
	if (life >= 5) { lifeSprite[5]->Draw(); }
	if (life >= 4) { lifeSprite[4]->Draw(); }
	if (life >= 3) { lifeSprite[3]->Draw(); }
	if (life >= 2) { lifeSprite[2]->Draw(); }
	if (life >= 1) { lifeSprite[1]->Draw(); }
	if (life >= 0) { lifeSprite[0]->Draw(); }
}