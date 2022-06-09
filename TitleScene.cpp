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
	//�X�v���C�g���ʃe�N�X�`���ǂݍ���
	SpriteCommon::GetInstance()->LoadTexture(1, L"Resources/title.png");

	//�X�v���C�g�̐���
	sprite = Sprite::Create(1, { 0,0 }, false, false);
	sprite->SetPosition({ 0,0,0 });

	//�J�����̏�����
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//�J������3D�I�u�W�F�b�g�ɃZ�b�g
	Object3d::SetCamera(camera);

	//camera->SetEye({ 0, 0, -50 });
	camera->SetTarget({ 0,0,0 });
	camera->SetDistance(100.0f);
}

void TitleScene::Finalize()
{
	delete camera;
	//�X�v���C�g�ʉ��
	delete sprite;
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();
	if (input->TriggerKey(DIK_SPACE))     // �X�y�[�X�L�[��������Ă�����
	{
		//�V�[���؂�ւ�
		BaseScene* scene = new GamePlayScene();
		this->sceneManager->SetNextScene(scene);
	}

	// ���W����
	/*if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{

	}*/

	//�X�V
	camera->Update();
	sprite->Update();
}

void TitleScene::Draw(DirectXCommon* dxCommon)
{
	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->PreDraw();
	//�X�v���C�g�`��
	sprite->Draw();
}