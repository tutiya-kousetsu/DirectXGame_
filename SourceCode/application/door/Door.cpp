#include "Door.h"
#include "Easing.h"
Door::Door()
{
}

Door::~Door()
{
}

void Door::Initialize()
{
	for (int i = 0; i < 8; i++) {
		object[i] = Object3d::Create();
		object[i]->SetModel(Model::CreateFromOBJ("door"));

		object[i]->SetScale({4.0f, 4.0f, 4.0f});
		position[i] = object[i]->GetPosition();
		position[0] = { 8, 6, 50 };
		position[1] = { -8, 6,50 };
		position[2] = { -50, 6, 8 };
		position[3] = { -50, 6,-8 };
		position[4] = { 50, 6, 8 };
		position[5] = { 50, 6,-8 };
		position[6] = { 8, 6, -50 };
		position[7] = { -8, 6,-50 };
		object[i]->SetPosition(position[i]);

		//ドアの向き
		rotation[i] = object[i]->GetRotation();
		rotation[2] = { 0, 90 ,0 };
		rotation[3] = { 0, 90 ,0 };
		rotation[4] = { 0, 90 ,0 };
		rotation[5] = { 0, 90 ,0 };
		object[i]->SetRotation(rotation[i]);
	}
}

void Door::Update()
{
	for (int i = 0; i < 8; i++) {
		object[i]->Update();
	}
}

void Door::DoorMove(int phaseCount)
{
	//前ドア(右)
	if (phaseCount >= 0) {
		if (outFrame[0] < 1.0f) {
			outFrame[0] += 0.005f;
		}
		position[0].x = Ease(Out, Cubic, outFrame[0], 8.0f, 0.0f);
	}
	//前ドア(左)
	if (phaseCount >= 1) {
		if (outFrame[1] < 1.0f) {
			outFrame[1] += 0.005f;
		}
		position[1].x = Ease(Out, Cubic, outFrame[1], -8.0f, -16.0f);
	}
	//左ドア(右)
	if (phaseCount >= 2) {
		if (outFrame[2] < 1.0f) {
			outFrame[2] += 0.005f;
		}
		position[2].z = Ease(Out, Cubic, outFrame[2], 8.0f, 0.0f);
	}
	//右ドア(右)
	if (phaseCount >= 3) {
		if (outFrame[5] < 1.0f) {
			outFrame[5] += 0.005f;
		}
		position[5].z = Ease(Out, Cubic, outFrame[5], -8.0f, -16.0f);
	}
	//後ろドア(左)
	if (phaseCount >= 4) {
		if (outFrame[6] < 1.0f) {
			outFrame[6] += 0.005f;
		}
		position[6].x = Ease(Out, Cubic, outFrame[6], 8.0f, 0.0f);
	}
	if (phaseCount >= 5) {
		//左ドア(左)
		if (outFrame[3] < 1.0f) {
			outFrame[3] += 0.005f;
		}
		position[3].z = Ease(Out, Cubic, outFrame[3], -8.0f, -16.0f);
		//右ドア(左)
		if (outFrame[4] < 1.0f) {
			outFrame[4] += 0.005f;
		}
		position[4].z = Ease(Out, Cubic, outFrame[4], 8.0f, 0.0f);
		//後ろドア(右)
		if (outFrame[7] < 1.0f) {
			outFrame[7] += 0.005f;
		}
		position[7].x = Ease(Out, Cubic, outFrame[7], -8.0f, -16.0f);
	}
	for (int i = 0; i < 8; i++) {
		object[i]->SetPosition(position[i]);
	}
}

void Door::Draw()
{
	for (int i = 0; i < 8; i++) {
		object[i]->Draw();
	}
}
