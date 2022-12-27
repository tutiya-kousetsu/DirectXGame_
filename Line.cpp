#include "Line.h"
Line::Line() :Line(Model::LoadFromObj("line"))
{
	object->SetScale({ 1.f, 1.f, 1.f });
}

void Line::Initialize(XMFLOAT3 pos)
{
	//“G‚ÌÀ•W“n‚·‚æ
	position = pos;
}

void Line::Update()
{
	//position = object->GetPosition();
	//position = { 0,0,0 };
	object->SetPosition(position);
	object->Update();
}

void Line::Draw()
{
	if (alive) {
		object->Draw();
	}
}
