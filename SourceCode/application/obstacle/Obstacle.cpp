#include "Obstacle.h"
#include "BaseCollider.h"
#include "CollisionManager.h"
#include "Easing.h"
#include <fstream>

Obstacle::~Obstacle()
{
	if (collider) {
		//�R���W�����}�l�[�W������o�^������
		CollisionManager::GetInstance()->RemoveCollider(collider);
		delete collider;
	}
}

bool Obstacle::Initialize(XMFLOAT3 position)
{
	object.reset(new TouchableObject());
	object->Initialize(Model::CreateFromOBJ("stone"));
	object->SetScale({ 3.4f, 3.21f, 3.4f });
	shake.reset(new Shake());
	this->position = position;
	//object->SetPosition(position);
	return true;

}

void Obstacle::Update()
{
	object->Update();
}

void Obstacle::Draw()
{
	object->Draw();
}

bool Obstacle::UpMove(bool landF)
{
	//���n������
	if (landF) {
		//�C�[�W���O�ŉ����グ��
		if (easFrame < 1.0f) {
			easFrame += 0.0001f;
		}
		position.y = Ease(Out, Quad, easFrame, position.y, 0);
		//�V�F�C�N�̃t���O�𗧂Ă�
		shake->SetShakeStart(true);
		//�|�W�V������y���W��-0.1�܂ōs������t���O��false�ɂ���
		if (position.y >= -0.1) {
			shake->SetShakeStart(false);
		}
	}
	//�V�F�C�N�̍ő�l�ŏ��l�Ȃǂ�����
	shake->ShakePos(shakePos.x, 1, -1, 10, 20);
	shake->ShakePos(shakePos.z, 1, -1, 10, 20);
	//�t���O��false��������V�F�C�N���Ƃ���
	if (!shake->GetShakeStart()) {
		shakePos = { 0.0f, 0.0f, 0.0f };
	}
	//�|�W�V�����ɃV�F�C�N�̓����𑫂�
	position.x += shakePos.x;
	position.z += shakePos.z;

	SetPosition(position);
	return false;
}

void Obstacle::DownMove(int32_t time)
{
	//���n������
	if (time <= 230) {
		//�C�[�W���O�ŉ����グ��
		if (easFrame < 1.0f) {
			easFrame += 0.0001f;
		}
		position.y = Ease(Out, Quad, easFrame, position.y, -10);
		//�V�F�C�N�̃t���O�𗧂Ă�
		shake->SetShakeStart(true);
		//�|�W�V������y���W��-0.1�܂ōs������t���O��false�ɂ���
		if (position.y <= -10) {
			shake->SetShakeStart(false);
		}
	}
	//�V�F�C�N�̍ő�l�ŏ��l�Ȃǂ�����
	shake->ShakePos(shakePos.x, 1, -1, 10, 20);
	shake->ShakePos(shakePos.z, 1, -1, 10, 20);
	//�t���O��false��������V�F�C�N���Ƃ���
	if (!shake->GetShakeStart()) {
		shakePos = { 0.0f, 0.0f, 0.0f };
	}
	//�|�W�V�����ɃV�F�C�N�̓����𑫂�
	position.x += shakePos.x;
	position.z += shakePos.z;

	SetPosition(position);
}

void Obstacle::OnCollision(bool flag)
{

	if (flag) {
		color = object->GetColor();
		color.w = 0.45f;
		object->SetColor({ 1.0f, 1.0f, 1.0f, color.w });
	}
	if (!flag) {
		object->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	}
}