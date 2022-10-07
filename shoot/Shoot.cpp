#include "Shoot.h"

Shoot::Shoot()
{
}

Shoot::~Shoot()
{
	delete(shootObj);

	delete(shootModel);
}

void Shoot::Initialize(Input* input, Player* player)
{
	//�����o�ϐ��ɃR�s�[
	this->input = input;
	this->player = player;
	//�f�[�^�ǂݍ���
	shootModel = Model::LoadFromObj("sphere");
	shootObj = Object3d::Create();
	shootObj->SetModel(shootModel);
	shootObj->SetScale({ 0.5f, 0.5f, 0.5f });
}

void Shoot::Update()
{
	Input* input = Input::GetInstance();
	if (aliveFlag == 1) {
		XMFLOAT3 position = shootObj->GetPosition();

		position.z += 0.5f;

		// ���W�̕ύX�𔽉f
		shootObj->SetPosition(position);

		//��ʂ͎��܂ōs������
		if (position.z > 33) {
			aliveFlag = 0;
		}
	}
	if (aliveFlag == 0) {
		//�X�y�[�X��������
		if (input->TriggerKey(DIK_SPACE)) {

			XMFLOAT3 position = player->GetPosition();

			// ���W�̕ύX�𔽉f
			shootObj->SetPosition({ position.x, position.y, position.z });

			aliveFlag = 1;
		}
	}
	shootObj->Update();
}

void Shoot::Draw()
{
	if (aliveFlag == 1) {
		shootObj->Draw();
	}
}
