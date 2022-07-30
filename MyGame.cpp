#include "MyGame.h"
#include "GamePlayScene.h"
#include "TitleScene.h"
void MyGame::Initialize()
{
	//���N���X�̏�����
	Framework::Initialize();

#pragma region �V�[������������
	BaseScene* scene = new TitleScene(SceneManager::GetInstance());
	//�V�[���}�l�[�W���[�ɍŏ��̃V�[�����Z�b�g
	SceneManager::GetInstance()->SetNextScene(scene);
}

void MyGame::Finalize()
{
	//���N���X�̏I������
	Framework::Finalize();
}

void MyGame::Update()
{
	//���N���X�̍X�V����
	Framework::Update();
}

void MyGame::Draw()
{
	//���N���X�̕`�揈��
	Framework::Draw();
}
