#include "Phase.h"
#include "Easing.h"

void Phase::Initialize()
{
	Sprite::LoadTexture(10, L"Resources/phase/phase1.png");
	phase[0].reset(Sprite::Create(10, { 1920, 360 }));
	Sprite::LoadTexture(11, L"Resources/phase/phase2.png");
	phase[1].reset(Sprite::Create(11, { 1920, 360 }));
	Sprite::LoadTexture(12, L"Resources/phase/phase3.png");
	phase[2].reset(Sprite::Create(12, { 1920, 360 }));
	Sprite::LoadTexture(13, L"Resources/phase/phase4.png");
	phase[3].reset(Sprite::Create(13, { 1920, 360 }));
	Sprite::LoadTexture(14, L"Resources/phase/phase5.png");
	phase[4].reset(Sprite::Create(14, { 1920, 360 }));
	Sprite::LoadTexture(15, L"Resources/phase/phase6.png");
	phase[5].reset(Sprite::Create(15, { 1920, 360 }));
}

void Phase::MovePhase(int i)
{
	pos[i] = phase[i]->GetPosition();
	//フェーズスプライトが右にあったら中心までイージングで動かす
	if (pos[i].x >= -1920) {
		if (outFrame < 1.0f) {
			outFrame += 0.01f;
		}
		pos[i].x = Ease(Out, Cubic, outFrame, 1920.0f, 640.0f);
		//フェーズスプライトが中心に来たら左に向かってイージングで動かす
		if (pos[i].x <= 640) {
			if (inFrame < 1.0f) {
				inFrame += 0.01f;
			}
			pos[i].x = Ease(In, Cubic, inFrame, 640.0f, -1920.0f);
		}
	}
	//スプライトが左の画面外に出たらフラグを立たせる
	else if (pos[i].x <= -1920) {
		phaseFlag = true;
	}
	//フラグが立ったら初期化
	if (phaseFlag) {
		outFrame = 0;
		inFrame = 0;
	}
	phase[i]->SetPosition(pos[i]);
}

void Phase::Draw(int i)
{
	phase[i]->Draw();
}