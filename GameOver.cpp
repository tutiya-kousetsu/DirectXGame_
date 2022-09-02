#include "GameOver.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "Input.h"

void GameOver::Initialize(DirectXCommon* dxCommon)
{
	//�X�v���C�g���ʃe�N�X�`���ǂݍ���
	SpriteCommon::GetInstance()->LoadTexture(1, L"Resources/GameOver.png");

	//�X�v���C�g�̐���
	sprite = Sprite::Create(1, { 0,0 }, false, false);
	sprite->SetPosition({ 0,0,0 });

}

void GameOver::Finalize()
{
	//�X�v���C�g�ʉ��
	delete sprite;
}

void GameOver::Update()
{
	Input* input = Input::GetInstance();
	if (input->TriggerKey(DIK_R))     // �X�y�[�X�L�[��������Ă�����
	{
		//�V�[���؂�ւ�
		BaseScene* scene = new TitleScene();
		SceneManager::GetInstance()->SetNextScene(scene);
	}

	//�X�V
	sprite->Update();
}

void GameOver::Draw(DirectXCommon* dxCommon)
{
	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->PreDraw();
	//�X�v���C�g�`��
	sprite->Draw();
}
