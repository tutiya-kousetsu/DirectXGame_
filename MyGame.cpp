#include "MyGame.h"
#include "GamePlayScene.h"
void MyGame::Initialize()
{
	//���N���X�̏�����
	Framework::Initialize();

#pragma region �V�[������������
	this->scene = new GamePlayScene();
	this->scene->Initialize(dxCommon);
}

void MyGame::Finalize()
{
	this->scene->Finalize();
	//���N���X�̏I������
	Framework::Finalize();
}

void MyGame::Update()
{
	//���N���X�̍X�V����
	Framework::Update();

	this->scene->Update();
}

void MyGame::Draw()
{
	//�`��O����
	dxCommon->PreDraw();

	//�V�[���`��
	this->scene->Draw();

	//�f�o�b�N�e�L�X�g�`��
	debugText->DrawAll();

	//�`��㏈��
	dxCommon->PostDraw();
}
