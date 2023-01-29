#include "Line.h"
Line::Line() :Line(Model::CreateFromOBJ("line"))
{
	object->SetScale({ 1.f, 1.f, 1.f });
}

void Line::Initialize(XMFLOAT3 pos)
{
	//�G�̍��W�n����
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
