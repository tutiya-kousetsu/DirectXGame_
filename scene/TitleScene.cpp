#include "TitleScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
#include "GamePlayScene.h"

void TitleScene::Initialize(DirectXCommon* dxCommon)
{
	//�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/Title.png");
	//�X�v���C�g�̐���
	titleSp = Sprite::Create(1, { 0.0f,0.0f });
	titleSp->SetPosition({ 0,0 });
	//�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(2, L"Resources/space.png");
	spaceSp = Sprite::Create(2, { 0.0f,0.0f });
	spaceSp->SetPosition({ 0,0 });

}

void TitleScene::Finalize()
{
	//�X�v���C�g�ʉ��
	delete titleSp;
	delete spaceSp;
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();
	//�}�E�X�̍��N���b�N��������Ă�����
	if (input->TriggerKey(DIK_SPACE))
	{
		//�V�[���؂�ւ�
		BaseScene* scene = new GamePlayScene();
		this->sceneManager->SetNextScene(scene);
	}
}

void TitleScene::Draw(DirectXCommon* dxCommon)
{
	//�`��O����
	dxCommon->PreDraw();

	//�X�v���C�g���ʃR�}���h
	Sprite::PreDraw(dxCommon->GetCmdList());
	//�X�v���C�g�`��
	titleSp->Draw();
	spaceSp->Draw();
	Sprite::PostDraw();
		//�`��㏈��
	dxCommon->PostDraw();

}