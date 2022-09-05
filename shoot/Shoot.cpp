#include "Shoot.h"

Shoot::Shoot()
{
}

Shoot::~Shoot()
{
	for (int i = 0; i < 5; i++) {
		delete(shootObj[i]);
	}

	delete(shootModel);
}

void Shoot::Initialize(Input* input, Player* player)
{
	//�����o�ϐ��ɃR�s�[
	this->input = input;
	this->player = player;
	//�f�[�^�ǂݍ���
	for (int i = 0; i < 5; i++) {
		shootModel = Model::LoadFromObj("sphere");
		shootObj[i] = Object3d::Create();
		shootObj[i]->SetModel(shootModel);
		shootObj[i]->SetScale({ 0.5f, 0.5f, 0.5f });
	}
}

void Shoot::Update()
{
	Input* input = Input::GetInstance();
	for (int i = 0; i < 5; i++) {
		if (aliveFlag == 1) {
			XMFLOAT3 position = shootObj[i]->GetPosition();

			position.z += 0.5f;

			// ���W�̕ύX�𔽉f
			shootObj[i]->SetPosition(position);

			//��ʂ͎��܂ōs������
			if (position.z > 33) {
				aliveFlag = 0;
			}
		}
		if (aliveFlag == 0) {
			//�X�y�[�X��������
			if (input->PushKey(DIK_SPACE)) {

				XMFLOAT3 position = player->GetPosition();

				// ���W�̕ύX�𔽉f
				shootObj[i]->SetPosition({ position.x, position.y, position.z });

				aliveFlag = 1;
			}
		}
		shootObj[i]->Update();
	}
}

void Shoot::Draw()
{
	if (aliveFlag == 1) {
		for (int i = 0; i < 5; i++) {
			shootObj[i]->Draw();
		}
	}
}
