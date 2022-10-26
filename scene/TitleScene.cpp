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
	Sprite::LoadTexture(1, L"Resources/title.png");
	//�X�v���C�g�̐���
	sprite = Sprite::Create(1, { 0.0f,0.0f });
	sprite->SetPosition({ 0,0 });

}

void TitleScene::Finalize()
{
	//�X�v���C�g�ʉ��
	delete sprite;
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();
	if (input->PushMouseLeft())     // �}�E�X�̍��N���b�N��������Ă�����
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
	sprite->Draw();
	Sprite::PostDraw();
		//�`��㏈��
	dxCommon->PostDraw();

}