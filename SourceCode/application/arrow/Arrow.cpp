#include "Arrow.h"

void Arrow::Initialize()
{
	//ª
	Sprite::LoadTexture(20, L"Resources/arrow.png");
	arrow[0].reset(Sprite::Create(20, { 750,0 }));
	Sprite::LoadTexture(21, L"Resources/arrow.png");
	arrow[1].reset(Sprite::Create(21, { 450,0 }));
	//©
	Sprite::LoadTexture(22, L"Resources/arrow.png");
	arrow[2].reset(Sprite::Create(22, { 0,280 }));
	arrow[2]->SetRotation(-90.0f);
	Sprite::LoadTexture(23, L"Resources/arrow.png");
	arrow[3].reset(Sprite::Create(23, { 0,480 }));
	arrow[3]->SetRotation(-90.0f);
	//¨
	Sprite::LoadTexture(24, L"Resources/arrow.png");
	arrow[4].reset(Sprite::Create(24, { 1280, 180 }));
	arrow[4]->SetRotation(90.0f);
	Sprite::LoadTexture(25, L"Resources/arrow.png");
	arrow[5].reset(Sprite::Create(25, { 1280,420 }));
	arrow[5]->SetRotation(90.0f);
	//«
	Sprite::LoadTexture(26, L"Resources/arrow.png");
	arrow[6].reset(Sprite::Create(26, { 750,720 }));
	arrow[6]->SetRotation(180.0f);
	Sprite::LoadTexture(27, L"Resources/arrow.png");
	arrow[7].reset(Sprite::Create(27, { 450,720 }));
	arrow[7]->SetRotation(180.0f);
}

void Arrow::MovePhase(int phaseCount)
{

}

void Arrow::Draw(int phaseCount)
{
	if(phaseCount >= 1){
		arrow[0]->Draw();
		arrow[1]->Draw();
	}
	if (phaseCount >= 2) {
		arrow[2]->Draw();
	}
	if (phaseCount >= 3) {
		arrow[5]->Draw();
	}
	if (phaseCount >= 4) {
		arrow[6]->Draw();
	}
	if (phaseCount >= 5) {
		arrow[3]->Draw();
		arrow[4]->Draw();
		arrow[7]->Draw();
	}
}
