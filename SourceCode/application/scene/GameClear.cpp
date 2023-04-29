#include "GameClear.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "Input.h"

void GameClear::Initialize(DirectXCommon* dxCommon)
{
	Sprite::LoadTexture(1, L"Resources/GameClear.png");
	//�X�v���C�g�̐���
	sprite = Sprite::Create(1, { 0,0 });
	sprite->SetPosition({ 0,0 });

}

void GameClear::Finalize()
{
	//�X�v���C�g�ʉ��
	delete sprite;
}

void GameClear::Update()
{
	Input* input = Input::GetInstance();
	if (input->TriggerKey(DIK_SPACE))     // �X�y�[�X�L�[��������Ă�����
	{
		//�V�[���؂�ւ�
		BaseScene* scene = new TitleScene();
		this->sceneManager->SetNextScene(scene);
	}
}

void GameClear::Draw(DirectXCommon* dxCommon)
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
