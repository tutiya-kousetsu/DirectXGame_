#include "Circle.h"
#include "easing/Easing.h"
void Circle::Initialize()
{
	Sprite::LoadTexture(1, L"Resources/circle.png");
	sprite.reset(Sprite::Create(1, { 0,0 }));
}

void Circle::Update()
{
	
}

void Circle::UpSize(int32_t time)
{
	if (time >= 0) {
		size = sprite->GetSize();
		/*if (xFrame < 1.0f) {
			xFrame += 0.01f;
		}
		size.x = Ease(Out, Cubic, xFrame, 8.0f, 0.0f);
		if (yFrame < 1.0f) {
			yFrame += 0.01f;
		}
		size.y = Ease(Out, Cubic, yFrame, 8.0f, 0.0f);
		sprite->SetSize(size);*/
		size.x++;
		size.y++;
		sprite->SetSize(size);
	}
}

void Circle::DownSize(int32_t time)
{
	if (time >= 0) {
		size = sprite->GetSize();
		size.x--;
		size.y--;
		sprite->SetSize(size);
	}
}

void Circle::Draw()
{

	//sprite->Draw();
}