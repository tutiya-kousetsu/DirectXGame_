#include "Phase.h"
#include "easing/Easing.h"

void Phase::Initialize()
{
	Sprite::LoadTexture(10, L"Resources/phase/phase1.png");
	phase[0].reset(Sprite::Create(10, { 1280, 0 }));
	Sprite::LoadTexture(11, L"Resources/phase/phase2.png");
	phase[1].reset(Sprite::Create(11, { 1280, 0 }));
	Sprite::LoadTexture(12, L"Resources/phase/phase3.png");
	phase[2].reset(Sprite::Create(12, { 1280, 0 }));
	Sprite::LoadTexture(13, L"Resources/phase/phase4.png");
	phase[3].reset(Sprite::Create(13, { 1280, 0 }));
	Sprite::LoadTexture(14, L"Resources/phase/phase5.png");
	phase[4].reset(Sprite::Create(14, { 1280, 0 }));
	Sprite::LoadTexture(15, L"Resources/phase/phase6.png");
	phase[5].reset(Sprite::Create(15, { 1280, 0 }));
}

void Phase::MovePhase(int i)
{
	pos[i] = phase[i]->GetPosition();
	if (pos[i].x >= -1280) {
		if (outFrame < 1.0f) {
			outFrame += 0.01f;
		}
		pos[i].x = Ease(InOut, Cubic, outFrame, 1280.0f, 0.0f);
		if (pos[i].x <= 0) {
			if (inFrame < 1.0f) {
				inFrame += 0.01f;
			}
			pos[i].x = Ease(In, Cubic, inFrame, 0.0f, -1280.0f);
		}
	}
	else if (pos[i].x <= -1280) {
		phaseFlag = true;
	}
	if (phaseFlag) {
		outFrame = 0;
		inFrame = 0;
	}
	phase[i]->SetPosition(pos[i]);
}

void Phase::Draw(int i)
{
	//for (auto i = 0; i <= 6; i++) {
		phase[i]->Draw();
	//}
	/*phase2->Draw();
	phase3->Draw();
	phase4->Draw();
	phase5->Draw();
	phase6->Draw();*/
}