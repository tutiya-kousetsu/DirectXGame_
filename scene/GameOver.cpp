#include "GameOver.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "Input.h"

void GameOver::Initialize(DirectXCommon* dxCommon)
{
	Sprite::LoadTexture(1, L"Resources/GameOver.png");
	//�X�v���C�g�̐���
	sprite = Sprite::Create(1, { 0.0f,0.0f });
	sprite->SetPosition({ 0,0 });

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
		this->sceneManager->SetNextScene(scene);
	}
}

void GameOver::Draw(DirectXCommon* dxCommon)
{
	//�`��O����
	dxCommon->PreDraw();

	//�X�v���C�g���ʃR�}���h
	Sprite::PreDraw(dxCommon->GetCmdList());
	//�X�v���C�g�`��
	sprite->Draw();
	Sprite::PostDraw();
	//�`��㏈��
	dxCommon->PostDraw();

}
