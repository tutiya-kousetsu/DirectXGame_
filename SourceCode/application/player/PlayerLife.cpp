#include "PlayerLife.h"
#include "Easing.h"
void PlayerLife::Initialize()
{
	Sprite::LoadTexture(2, L"Resources/HP.png");
	lifeSp[0].reset(Sprite::Create(2, { 930,595 }));
	Sprite::LoadTexture(3, L"Resources/HP.png");
	lifeSp[1].reset(Sprite::Create(3, { 970,595 }));
	Sprite::LoadTexture(4, L"Resources/HP.png");
	lifeSp[2].reset(Sprite::Create(4, { 1010,595 }));
	Sprite::LoadTexture(5, L"Resources/HP.png");
	lifeSp[3].reset(Sprite::Create(5, { 1050,595 }));
	Sprite::LoadTexture(6, L"Resources/HP.png");
	lifeSp[4].reset(Sprite::Create(6, { 1090,595 }));
	Sprite::LoadTexture(7, L"Resources/HP.png");
	lifeSp[5].reset(Sprite::Create(7, { 1130,595 }));
	Sprite::LoadTexture(8, L"Resources/HP.png");
	lifeSp[6].reset(Sprite::Create(8, { 1170,595 }));
	Sprite::LoadTexture(9, L"Resources/HP.png");
	lifeSp[7].reset(Sprite::Create(9, { 1210,595 }));

}

void PlayerLife::MoveUpdate(int life)
{
	for (int i = 0; i < life; i++) {
		pos[i] = lifeSp[i]->GetPosition();
		//if (pos[i].y <= 900) {
		if (frame < 1.0f) {
			frame += 0.001f;
		}
		pos[i].y = Ease(Out, Cubic, frame, pos[i].y, 900);

		//}
		lifeSp[i]->SetPosition(pos[i]);


		if (pos[i].y >= 900) {
			disFlag = true;
			frame = 0;
		}
	}
}

void PlayerLife::Draw(int life)
{
	for (int i = 0; i < life; i++) {
		lifeSp[i]->Draw();
	}
}
